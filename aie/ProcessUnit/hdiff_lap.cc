#include <adf.h>
#include "include.h"
#include "hdiff.h"

using namespace adf;

#define kernel_load 14

alignas(32) static const int32_t weights[8]      = {-4, -4, -4, -4, -4, -4, -4, -4};
alignas(32) static const int32_t weights_rest[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

void hdiff_lap(input_window_int32* row0_win,
               input_window_int32* row1_win,
               input_window_int32* row2_win,
               input_window_int32* row3_win,
               input_window_int32* row4_win,
               output_window_int32* out_flux1_win,
               output_window_int32* out_flux2_win,
               output_window_int32* out_flux3_win,
               output_window_int32* out_flux4_win) {
  v8int32 coeffs      = *(const v8int32*)weights;
  v8int32 coeffs_rest = *(const v8int32*)weights_rest;

  v16int32 data_buf1 = null_v16int32();
  v16int32 data_buf2 = null_v16int32();

  v8acc80 acc_0 = null_v8acc80();
  v8acc80 acc_1 = null_v8acc80();

  v8int32 lap_ij = null_v8int32();
  v8int32 lap_0  = null_v8int32();

  data_buf1 = upd_w(data_buf1, 0, window_readincr_v8(row3_win));
  data_buf1 = upd_w(data_buf1, 1, window_read_v8(row3_win));

  data_buf2 = upd_w(data_buf2, 0, window_readincr_v8(row1_win));
  data_buf2 = upd_w(data_buf2, 1, window_read_v8(row1_win));

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining
    chess_loop_range(1, ) {
      v16int32 flux_sub;

      // lap_ij and lap_ijm
      acc_0 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_1 = lmul8(data_buf2, 1, 0x76543210, coeffs_rest, 0, 0x00000000);

      acc_0 = lmac8(acc_0, data_buf1, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf1, 1, 0x76543210, coeffs_rest, 0, 0x00000000);

      data_buf2 = upd_w(data_buf2, 0, window_readincr_v8(row2_win));
      data_buf2 = upd_w(data_buf2, 1, window_read_v8(row2_win));

      acc_0 = lmac8(acc_0, data_buf2, 1, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 2, 0x76543210, coeffs,      0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 3, 0x76543210, coeffs_rest, 0, 0x00000000);

      lap_ij = srs(acc_0, 0);

      acc_1 = lmac8(acc_1, data_buf2, 0, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 1, 0x76543210, coeffs,      0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);

      lap_0 = srs(acc_1, 0);

      flux_sub =
          sub16(concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_0,  undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      window_writeincr(out_flux1_win, ext_w(flux_sub, 0));

      // lap_ijp
      acc_0 = lmul8(data_buf1, 3, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 3, 0x76543210, coeffs,      0, 0x00000000);

      window_decr_v8(row1_win, 1);
      data_buf1 = upd_w(data_buf1, 0, window_readincr_v8(row1_win));
      data_buf1 = upd_w(data_buf1, 1, window_read_v8(row1_win));

      acc_0 = lmac8(acc_0, data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 4, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf1, 3, 0x76543210, coeffs_rest, 0, 0x00000000);

      lap_0 = srs(acc_0, 0);

      flux_sub =
          sub16(concat(lap_0,  undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      window_writeincr(out_flux2_win, ext_w(flux_sub, 0));

      // lap_imj and lap_ipj
      acc_1 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);

      data_buf2 = upd_w(data_buf2, 0, window_readincr_v8(row0_win));
      data_buf2 = upd_w(data_buf2, 1, window_read_v8(row0_win));

      acc_1 = lmsc8(acc_1, data_buf1, 2, 0x76543210, coeffs,      0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf1, 1, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);

      data_buf2 = upd_w(data_buf2, 0, window_readincr_v8(row4_win));
      data_buf2 = upd_w(data_buf2, 1, window_read_v8(row4_win));

      acc_1 = lmac8(acc_1, data_buf1, 3, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);

      lap_0 = srs(acc_1, 0);

      flux_sub =
          sub16(concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_0,  undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      window_writeincr(out_flux3_win, ext_w(flux_sub, 0));

      window_decr_v8(row3_win, 1);
      data_buf1 = upd_w(data_buf1, 0, window_readincr_v8(row3_win));
      data_buf1 = upd_w(data_buf1, 1, window_read_v8(row3_win));

      acc_0 = lmsc8(acc_0, data_buf1, 2, 0x76543210, coeffs,      0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf1, 1, 0x76543210, coeffs_rest, 0, 0x00000000);

      data_buf2 = upd_w(data_buf2, 0, window_readincr_v8(row1_win));
      data_buf2 = upd_w(data_buf2, 1, window_read_v8(row1_win));

      acc_0 = lmac8(acc_0, data_buf1, 3, 0x76543210, coeffs_rest, 0, 0x00000000);

      flux_sub =
          sub16(concat(srs(acc_0, 0), undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_ij,        undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      window_writeincr(out_flux4_win, ext_w(flux_sub, 0));

      data_buf1 = upd_w(data_buf1, 0, window_readincr_v8(row3_win));
      data_buf1 = upd_w(data_buf1, 1, window_read_v8(row3_win));
    }
}