#include <adf.h>
#include "include.h"
#include "hdiff.h"

using namespace adf;

#define kernel_load 14

void hdiff_flux1(input_window_int32* row1_win,
                 input_window_int32* row2_win,
                 input_window_int32* row3_win,
                 input_window_int32* flux_forward1_win,
                 input_window_int32* flux_forward2_win,
                 input_window_int32* flux_forward3_win,
                 input_window_int32* flux_forward4_win,
                 output_window_int32* flux_inter1_win,
                 output_window_int32* flux_inter2_win,
                 output_window_int32* flux_inter3_win,
                 output_window_int32* flux_inter4_win,
                 output_window_int32* flux_inter5_win) {
  // preload two adjacent v8 chunks for each row
  v8int32 row1_cur  = window_readincr_v8(row1_win);
  v8int32 row1_next = window_readincr_v8(row1_win);

  v8int32 row2_cur  = window_readincr_v8(row2_win);
  v8int32 row2_next = window_readincr_v8(row2_win);

  v8int32 row3_cur  = window_readincr_v8(row3_win);
  v8int32 row3_next = window_readincr_v8(row3_win);

  v16int32 row2_pair = null_v16int32();
  v16int32 pair = null_v16int32();
  v8int32 flux_sub = null_v8int32();
  v8acc80 acc = null_v8acc80();

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining
    chess_loop_range(1, ) {
      row2_pair = null_v16int32();
      row2_pair = upd_w(row2_pair, 0, row2_cur);
      row2_pair = upd_w(row2_pair, 1, row2_next);

      // flux_inter1
      flux_sub = window_readincr_v8(flux_forward1_win);
      acc = lmul8(row2_pair, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc = lmsc8(acc,      row2_pair, 1, 0x76543210, flux_sub, 0, 0x00000000);
      window_writeincr(flux_inter1_win, flux_sub);
      window_writeincr(flux_inter1_win, srs(acc, 0));

      // flux_inter2
      flux_sub = window_readincr_v8(flux_forward2_win);
      acc = lmul8(row2_pair, 3, 0x76543210, flux_sub, 0, 0x00000000);
      acc = lmsc8(acc,      row2_pair, 2, 0x76543210, flux_sub, 0, 0x00000000);
      window_writeincr(flux_inter2_win, flux_sub);
      window_writeincr(flux_inter2_win, srs(acc, 0));

      // flux_inter3: uses row1
      pair = null_v16int32();
      pair = upd_w(pair, 0, row1_cur);
      pair = upd_w(pair, 1, row1_next);

      flux_sub = window_readincr_v8(flux_forward3_win);
      acc = lmul8(row2_pair, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc = lmsc8(acc,      pair,      2, 0x76543210, flux_sub, 0, 0x00000000);
      window_writeincr(flux_inter3_win, flux_sub);
      window_writeincr(flux_inter3_win, srs(acc, 0));

      // flux_inter4: uses row3
      pair = null_v16int32();
      pair = upd_w(pair, 0, row3_cur);
      pair = upd_w(pair, 1, row3_next);

      flux_sub = window_readincr_v8(flux_forward4_win);
      acc = lmul8(pair,      2, 0x76543210, flux_sub, 0, 0x00000000);
      acc = lmsc8(acc, row2_pair, 2, 0x76543210, flux_sub, 0, 0x00000000);
      window_writeincr(flux_inter4_win, flux_sub);
      window_writeincr(flux_inter4_win, srs(acc, 0));

      // flux_inter5
      window_writeincr(flux_inter5_win, ext_w(row2_pair, 1));
      window_writeincr(flux_inter5_win, ext_w(row2_pair, 0));

      // slide
      if (i != COL / 8 - 1) {
        row1_cur  = row1_next;
        row1_next = window_readincr_v8(row1_win);

        row2_cur  = row2_next;
        row2_next = window_readincr_v8(row2_win);

        row3_cur  = row3_next;
        row3_next = window_readincr_v8(row3_win);
      }
    }
}