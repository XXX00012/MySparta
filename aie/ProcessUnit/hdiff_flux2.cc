#include <adf.h>
#include "include.h"
#include "hdiff.h"

using namespace adf;

#define kernel_load 14

alignas(32) static const int32_t weights1[8] = {1, 1, 1, 1, 1, 1, 1, 1};
alignas(32) static const int32_t flux_out[8] = {-7, -7, -7, -7, -7, -7, -7, -7};

void hdiff_flux2(input_window_int32* flux_inter1_win,
                 input_window_int32* flux_inter2_win,
                 input_window_int32* flux_inter3_win,
                 input_window_int32* flux_inter4_win,
                 input_window_int32* flux_inter5_win,
                 output_window_int32* out_win) {
  v8int32 coeffs1        = *(const v8int32*)weights1;
  v8int32 flux_out_coeff = *(const v8int32*)flux_out;

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining
    chess_loop_range(1, ) {
      v8int32 flux_sub;
      v8int32 flux_interm_sub;

      // inter1
      flux_sub        = window_readincr_v8(flux_inter1_win);
      flux_interm_sub = window_readincr_v8(flux_inter1_win);

      unsigned int flx_compare_imj =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter1 =
          select16(flx_compare_imj,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      // inter2
      flux_sub        = window_readincr_v8(flux_inter2_win);
      flux_interm_sub = window_readincr_v8(flux_inter2_win);

      unsigned int flx_compare_ij =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter2 =
          select16(flx_compare_ij,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out2 = sub16(out_flx_inter2, out_flx_inter1);

      // inter3
      flux_sub        = window_readincr_v8(flux_inter3_win);
      flux_interm_sub = window_readincr_v8(flux_inter3_win);

      unsigned int fly_compare_ijm =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter3 =
          select16(fly_compare_ijm,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out3 = sub16(flx_out2, out_flx_inter3);

      // inter4
      flux_sub        = window_readincr_v8(flux_inter4_win);
      flux_interm_sub = window_readincr_v8(flux_inter4_win);

      unsigned int fly_compare_ij =
          gt16(concat(flux_interm_sub, undef_v8int32()), 0, 0x76543210,
               0xFEDCBA98, null_v16int32(), 0, 0x76543210, 0xFEDCBA98);

      v16int32 out_flx_inter4 =
          select16(fly_compare_ij,
                   concat(flux_sub, undef_v8int32()),
                   null_v16int32());

      v16int32 flx_out4 = add16(flx_out3, out_flx_inter4);

      // inter5
      v8int32 tmp1 = window_readincr_v8(flux_inter5_win);
      v8int32 tmp2 = window_readincr_v8(flux_inter5_win);
      v16int32 data_buf2 = concat(tmp2, tmp1);

      v8acc80 final_output =
          lmul8(flx_out4, 0, 0x76543210, flux_out_coeff, 0, 0x00000000);
      final_output =
          lmac8(final_output, data_buf2, 2, 0x76543210,
                concat(coeffs1, undef_v8int32()), 0, 0x76543210);

      window_writeincr(out_win, srs(final_output, 0));
    }
}