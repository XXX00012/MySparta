#include <adf.h>
#include <aie_api/aie.hpp>
#include <cstdint>
#include "ProcessUnit/include.h"
#include "ProcessUnit/hdiff.h"

using namespace adf;

#define kernel_load 14

namespace {

constexpr int INTER_DATA_WORDS = 2 * COL;
constexpr int FINAL_DATA_WORDS = COL;

inline void store_u64_to_i32_tail(int32_t* p, uint64_t v) {
  p[0] = static_cast<int32_t>(static_cast<uint32_t>(v & 0xffffffffULL));
  p[1] = static_cast<int32_t>(static_cast<uint32_t>((v >> 32) & 0xffffffffULL));
}

inline uint64_t load_u64_from_i32_tail(const int32_t* p) {
  return (static_cast<uint64_t>(static_cast<uint32_t>(p[0]))      ) |
         (static_cast<uint64_t>(static_cast<uint32_t>(p[1])) << 32);
}

} // namespace

void hdiff_flux2(input_buffer<int32_t>& flux_inter1,
                 input_buffer<int32_t>& flux_inter2,
                 input_buffer<int32_t>& flux_inter3,
                 input_buffer<int32_t>& flux_inter4,
                 input_buffer<int32_t>& flux_inter5,
                 output_buffer<int32_t>& out) {

  alignas(32) int32_t weights1[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  alignas(32) int32_t flux_out_arr[8] = {-7, -7, -7, -7, -7, -7, -7, -7};

  v8int32 coeffs1        = *(v8int32*)weights1;
  v8int32 flux_out_coeff = *(v8int32*)flux_out_arr;

  v8int32* ptr_in = nullptr;
  v8int32* ptr_out = nullptr;

  const int32_t* upstream_meta = flux_inter1.data() + INTER_DATA_WORDS;
  const uint64_t lap_start   = load_u64_from_i32_tail(upstream_meta + 0);
  const uint64_t lap_end     = load_u64_from_i32_tail(upstream_meta + 2);
  const uint64_t flux1_start = load_u64_from_i32_tail(upstream_meta + 4);
  const uint64_t flux1_end   = load_u64_from_i32_tail(upstream_meta + 6);

  aie::tile tile = aie::tile::current();
  const uint64_t kernel_start = tile.cycles();

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining
    chess_loop_range(1, ) {
      v8int32 flux_sub;
      v8int32 flux_interm_sub;

      // inter1
      ptr_in = (v8int32*)flux_inter1.data() + (i * 2);
      flux_sub        = *ptr_in++;
      flux_interm_sub = *ptr_in;

      unsigned int flx_compare_imj =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter1 =
          select16(flx_compare_imj,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      // inter2
      ptr_in = (v8int32*)flux_inter2.data() + (i * 2);
      flux_sub        = *ptr_in++;
      flux_interm_sub = *ptr_in;

      unsigned int flx_compare_ij =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter2 =
          select16(flx_compare_ij,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out2 = sub16(out_flx_inter2, out_flx_inter1);

      // inter3
      ptr_in = (v8int32*)flux_inter3.data() + (i * 2);
      flux_sub        = *ptr_in++;
      flux_interm_sub = *ptr_in;

      unsigned int fly_compare_ijm =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter3 =
          select16(fly_compare_ijm,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out3 = sub16(flx_out2, out_flx_inter3);

      // inter4
      ptr_in = (v8int32*)flux_inter4.data() + (i * 2);
      flux_sub        = *ptr_in++;
      flux_interm_sub = *ptr_in;

      unsigned int fly_compare_ij =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter4 =
          select16(fly_compare_ij,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out4 = add16(flx_out3, out_flx_inter4);

      // inter5
      ptr_in = (v8int32*)flux_inter5.data() + (i * 2);
      v8int32 tmp1 = *ptr_in++;
      v8int32 tmp2 = *ptr_in;
      v16int32 data_buf2 = concat(tmp2, tmp1);

      v8acc80 final_output =
          lmul8(flx_out4, 0, 0x76543210, flux_out_coeff, 0, 0x00000000);
      final_output =
          lmac8(final_output, data_buf2, 2, 0x76543210,
                concat(coeffs1, undef_v8int32()), 0, 0x76543210);

      ptr_out = (v8int32*)out.data() + i;
      *ptr_out = srs(final_output, 0);
    }

  const uint64_t kernel_end = tile.cycles();

  int32_t* meta = out.data() + FINAL_DATA_WORDS;
  store_u64_to_i32_tail(meta + 0,  lap_start);
  store_u64_to_i32_tail(meta + 2,  lap_end);
  store_u64_to_i32_tail(meta + 4,  flux1_start);
  store_u64_to_i32_tail(meta + 6,  flux1_end);
  store_u64_to_i32_tail(meta + 8,  kernel_start);
  store_u64_to_i32_tail(meta + 10, kernel_end);
}