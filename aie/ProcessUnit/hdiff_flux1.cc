#include <adf.h>
#include <aie_api/aie.hpp>
#include <cstdint>
#include "ProcessUnit/include.h"
#include "ProcessUnit/hdiff.h"

using namespace adf;

#define kernel_load 14

namespace {

constexpr int LAP_DATA_WORDS   = COL;
constexpr int INTER_DATA_WORDS = 2 * COL;

inline void store_u64_to_i32_tail(int32_t* p, uint64_t v) {
  p[0] = static_cast<int32_t>(static_cast<uint32_t>(v & 0xffffffffULL));
  p[1] = static_cast<int32_t>(static_cast<uint32_t>((v >> 32) & 0xffffffffULL));
}

inline uint64_t load_u64_from_i32_tail(const int32_t* p) {
  return (static_cast<uint64_t>(static_cast<uint32_t>(p[0]))      ) |
         (static_cast<uint64_t>(static_cast<uint32_t>(p[1])) << 32);
}

inline void store_inter_meta(int32_t* base,
                             uint64_t lap_start,
                             uint64_t lap_end,
                             uint64_t flux1_start,
                             uint64_t flux1_end) {
  store_u64_to_i32_tail(base + 0, lap_start);
  store_u64_to_i32_tail(base + 2, lap_end);
  store_u64_to_i32_tail(base + 4, flux1_start);
  store_u64_to_i32_tail(base + 6, flux1_end);
}

} // namespace

void hdiff_flux1(input_buffer<int32_t>& row1,
                 input_buffer<int32_t>& row2,
                 input_buffer<int32_t>& row3,
                 input_buffer<int32_t>& flux_forward1,
                 input_buffer<int32_t>& flux_forward2,
                 input_buffer<int32_t>& flux_forward3,
                 input_buffer<int32_t>& flux_forward4,
                 output_buffer<int32_t>& flux_inter1,
                 output_buffer<int32_t>& flux_inter2,
                 output_buffer<int32_t>& flux_inter3,
                 output_buffer<int32_t>& flux_inter4,
                 output_buffer<int32_t>& flux_inter5) {

  v8int32* __restrict row1_ptr = (v8int32*)row1.data();
  v8int32* __restrict row2_ptr = (v8int32*)row2.data();
  v8int32* __restrict row3_ptr = (v8int32*)row3.data();

  v8int32* ptr_forward = nullptr;
  v8int32* ptr_out = nullptr;

  v16int32 data_buf1 = null_v16int32();
  v16int32 data_buf2 = null_v16int32();

  v8acc80 acc_0 = null_v8acc80();
  v8acc80 acc_1 = null_v8acc80();

  const int32_t* lap_meta = flux_forward1.data() + LAP_DATA_WORDS;
  const uint64_t lap_start = load_u64_from_i32_tail(lap_meta + 0);
  const uint64_t lap_end   = load_u64_from_i32_tail(lap_meta + 2);

  aie::tile tile = aie::tile::current();
  const uint64_t kernel_start = tile.cycles();

  // Preload
  data_buf1 = upd_w(data_buf1, 0, *row1_ptr++);
  data_buf1 = upd_w(data_buf1, 1, *row1_ptr);

  data_buf2 = upd_w(data_buf2, 0, *row2_ptr++);
  data_buf2 = upd_w(data_buf2, 1, *row2_ptr);

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining
    chess_loop_range(1, ) {
      v8int32 flux_sub;

      // flux_inter1
      ptr_forward = (v8int32*)flux_forward1.data() + i;
      flux_sub = *ptr_forward;

      acc_1 = lmul8(data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 1, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32*)flux_inter1.data() + (i * 2);
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      // flux_inter2
      ptr_forward = (v8int32*)flux_forward2.data() + i;
      flux_sub = *ptr_forward;

      acc_0 = lmul8(data_buf2, 3, 0x76543210, flux_sub, 0, 0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32*)flux_inter2.data() + (i * 2);
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_0, 0);

      // flux_inter3
      ptr_forward = (v8int32*)flux_forward3.data() + i;
      flux_sub = *ptr_forward;

      acc_1 = lmul8(data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf1, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32*)flux_inter3.data() + (i * 2);
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      // row3 pair for flux_inter4
      row3_ptr = ((v8int32*)(row3.data())) + i;
      data_buf1 = upd_w(data_buf1, 0, *row3_ptr++);
      data_buf1 = upd_w(data_buf1, 1, *row3_ptr);

      // flux_inter4
      ptr_forward = (v8int32*)flux_forward4.data() + i;
      flux_sub = *ptr_forward;

      acc_1 = lmul8(data_buf1, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32*)flux_inter4.data() + (i * 2);
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      // reload next row1 pair
      row1_ptr = ((v8int32*)(row1.data())) + i + 1;
      data_buf1 = upd_w(data_buf1, 0, *row1_ptr++);
      data_buf1 = upd_w(data_buf1, 1, *row1_ptr);

      // flux_inter5
      ptr_out = (v8int32*)flux_inter5.data() + (i * 2);
      *ptr_out++ = ext_w(data_buf2, 1);
      *ptr_out = ext_w(data_buf2, 0);

      // reload next row2 pair
      row2_ptr = ((v8int32*)(row2.data())) + i + 1;
      data_buf2 = upd_w(data_buf2, 0, *row2_ptr++);
      data_buf2 = upd_w(data_buf2, 1, *row2_ptr);
    }

  const uint64_t kernel_end = tile.cycles();

  store_inter_meta(flux_inter1.data() + INTER_DATA_WORDS, lap_start, lap_end, kernel_start, kernel_end);
  store_inter_meta(flux_inter2.data() + INTER_DATA_WORDS, lap_start, lap_end, kernel_start, kernel_end);
  store_inter_meta(flux_inter3.data() + INTER_DATA_WORDS, lap_start, lap_end, kernel_start, kernel_end);
  store_inter_meta(flux_inter4.data() + INTER_DATA_WORDS, lap_start, lap_end, kernel_start, kernel_end);
  store_inter_meta(flux_inter5.data() + INTER_DATA_WORDS, lap_start, lap_end, kernel_start, kernel_end);
}