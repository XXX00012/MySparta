//===- hdiff_flux1.cc -------------------------------------------*- C++ -*-===//
//
// (c) 2023 SAFARI Research Group at ETH Zurich, Gagandeep Singh, D-ITET
//
// This file is licensed under the MIT License.
// SPDX-License-Identifier: MIT
//
//===----------------------------------------------------------------------===//

#include "./include.h"
#include "hdiff.h"
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

  alignas(32) int32_t row1_buf[COL];
  alignas(32) int32_t row2_buf[COL];
  alignas(32) int32_t row3_buf[COL];

  alignas(32) int32_t flux_forward1_buf[COL];
  alignas(32) int32_t flux_forward2_buf[COL];
  alignas(32) int32_t flux_forward3_buf[COL];
  alignas(32) int32_t flux_forward4_buf[COL];

  alignas(32) int32_t flux_inter1_buf[2 * COL];
  alignas(32) int32_t flux_inter2_buf[2 * COL];
  alignas(32) int32_t flux_inter3_buf[2 * COL];
  alignas(32) int32_t flux_inter4_buf[2 * COL];
  alignas(32) int32_t flux_inter5_buf[2 * COL];

  for (int i = 0; i < COL; ++i) row1_buf[i] = window_readincr(row1_win);
  for (int i = 0; i < COL; ++i) row2_buf[i] = window_readincr(row2_win);
  for (int i = 0; i < COL; ++i) row3_buf[i] = window_readincr(row3_win);

  for (int i = 0; i < COL; ++i) flux_forward1_buf[i] = window_readincr(flux_forward1_win);
  for (int i = 0; i < COL; ++i) flux_forward2_buf[i] = window_readincr(flux_forward2_win);
  for (int i = 0; i < COL; ++i) flux_forward3_buf[i] = window_readincr(flux_forward3_win);
  for (int i = 0; i < COL; ++i) flux_forward4_buf[i] = window_readincr(flux_forward4_win);

  int32_t *restrict row1_base = row1_buf;
  int32_t *restrict row2_base = row2_buf;
  int32_t *restrict row3_base = row3_buf;

  int32_t *restrict flux_forward1_base = flux_forward1_buf;
  int32_t *restrict flux_forward2_base = flux_forward2_buf;
  int32_t *restrict flux_forward3_base = flux_forward3_buf;
  int32_t *restrict flux_forward4_base = flux_forward4_buf;

  int32_t *restrict flux_inter1_base = flux_inter1_buf;
  int32_t *restrict flux_inter2_base = flux_inter2_buf;
  int32_t *restrict flux_inter3_base = flux_inter3_buf;
  int32_t *restrict flux_inter4_base = flux_inter4_buf;
  int32_t *restrict flux_inter5_base = flux_inter5_buf;

  v8int32 *restrict ptr_forward = (v8int32 *)flux_forward1_base;
  v8int32 *ptr_out = (v8int32 *)flux_inter1_base;

  v8int32 *restrict row1_ptr = (v8int32 *)row1_base;
  v8int32 *restrict row2_ptr = (v8int32 *)row2_base;
  v8int32 *restrict row3_ptr = (v8int32 *)row3_base;

  v16int32 data_buf1 = null_v16int32();
  v16int32 data_buf2 = null_v16int32();

  v8acc80 acc_0 = null_v8acc80();
  v8acc80 acc_1 = null_v8acc80();

  data_buf1 = upd_w(data_buf1, 0, *row1_ptr++);
  data_buf1 = upd_w(data_buf1, 1, *row1_ptr);

  data_buf2 = upd_w(data_buf2, 0, *row2_ptr++);
  data_buf2 = upd_w(data_buf2, 1, *row2_ptr);

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining chess_loop_range(1, ) {
      v8int32 flux_sub;

      ptr_forward = (v8int32 *)flux_forward1_base + i;
      flux_sub = *ptr_forward;

      acc_1 = lmul8(data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 1, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32 *)flux_inter1_base + 2 * i;
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      ptr_forward = (v8int32 *)flux_forward2_base + i;
      flux_sub = *ptr_forward;

      acc_0 = lmul8(data_buf2, 3, 0x76543210, flux_sub, 0, 0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32 *)flux_inter2_base + 2 * i;
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_0, 0);

      ptr_forward = (v8int32 *)flux_forward3_base + i;
      flux_sub = *ptr_forward;
      acc_1 = lmul8(data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf1, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32 *)flux_inter3_base + 2 * i;
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      row3_ptr = ((v8int32 *)row3_base) + i;
      data_buf1 = upd_w(data_buf1, 0, *(row3_ptr)++);
      data_buf1 = upd_w(data_buf1, 1, *(row3_ptr));

      ptr_forward = (v8int32 *)flux_forward4_base + i;
      flux_sub = *ptr_forward;

      acc_1 = lmul8(data_buf1, 2, 0x76543210, flux_sub, 0, 0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 2, 0x76543210, flux_sub, 0, 0x00000000);

      ptr_out = (v8int32 *)flux_inter4_base + 2 * i;
      *ptr_out++ = flux_sub;
      *ptr_out = srs(acc_1, 0);

      row1_ptr = ((v8int32 *)row1_base) + i + 1;
      data_buf1 = upd_w(data_buf1, 0, *(row1_ptr)++);
      data_buf1 = upd_w(data_buf1, 1, *(row1_ptr));

      ptr_out = (v8int32 *)flux_inter5_base + 2 * i;
      *ptr_out++ = ext_w(data_buf2, 1);
      *ptr_out = ext_w(data_buf2, 0);

      row2_ptr = ((v8int32 *)row2_base) + i + 1;
      data_buf2 = upd_w(data_buf2, 0, *(row2_ptr)++);
      data_buf2 = upd_w(data_buf2, 1, *row2_ptr);
    }

  for (int i = 0; i < 2 * COL; ++i) window_writeincr(flux_inter1_win, flux_inter1_buf[i]);
  for (int i = 0; i < 2 * COL; ++i) window_writeincr(flux_inter2_win, flux_inter2_buf[i]);
  for (int i = 0; i < 2 * COL; ++i) window_writeincr(flux_inter3_win, flux_inter3_buf[i]);
  for (int i = 0; i < 2 * COL; ++i) window_writeincr(flux_inter4_win, flux_inter4_buf[i]);
  for (int i = 0; i < 2 * COL; ++i) window_writeincr(flux_inter5_win, flux_inter5_buf[i]);
}