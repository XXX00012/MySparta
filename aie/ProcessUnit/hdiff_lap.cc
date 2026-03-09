//===- hdiff_lap.cc ---------------------------------------------*- C++ -*-===//
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

void hdiff_lap(input_window_int32* row0_win,
               input_window_int32* row1_win,
               input_window_int32* row2_win,
               input_window_int32* row3_win,
               input_window_int32* row4_win,
               output_window_int32* out_flux1_win,
               output_window_int32* out_flux2_win,
               output_window_int32* out_flux3_win,
               output_window_int32* out_flux4_win) {

  alignas(32) int32_t row0_buf[COL];
  alignas(32) int32_t row1_buf[COL];
  alignas(32) int32_t row2_buf[COL];
  alignas(32) int32_t row3_buf[COL];
  alignas(32) int32_t row4_buf[COL];

  alignas(32) int32_t out_flux1_buf[COL];
  alignas(32) int32_t out_flux2_buf[COL];
  alignas(32) int32_t out_flux3_buf[COL];
  alignas(32) int32_t out_flux4_buf[COL];

  for (int i = 0; i < COL; ++i) row0_buf[i] = window_readincr(row0_win);
  for (int i = 0; i < COL; ++i) row1_buf[i] = window_readincr(row1_win);
  for (int i = 0; i < COL; ++i) row2_buf[i] = window_readincr(row2_win);
  for (int i = 0; i < COL; ++i) row3_buf[i] = window_readincr(row3_win);
  for (int i = 0; i < COL; ++i) row4_buf[i] = window_readincr(row4_win);

  int32_t *restrict row0_ptr_base = row0_buf;
  int32_t *restrict row1_ptr_base = row1_buf;
  int32_t *restrict row2_ptr_base = row2_buf;
  int32_t *restrict row3_ptr_base = row3_buf;
  int32_t *restrict row4_ptr_base = row4_buf;

  int32_t *restrict out_flux1_base = out_flux1_buf;
  int32_t *restrict out_flux2_base = out_flux2_buf;
  int32_t *restrict out_flux3_base = out_flux3_buf;
  int32_t *restrict out_flux4_base = out_flux4_buf;

  alignas(32) int32_t weights[8] = {-4, -4, -4, -4, -4, -4, -4, -4};
  alignas(32) int32_t weights1[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  alignas(32) int32_t weights_rest[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
  alignas(32) int32_t flux_out[8] = {-7, -7, -7, -7, -7, -7, -7, -7};

  v8int32 coeffs = *(v8int32 *)weights;
  v8int32 coeffs1 = *(v8int32 *)weights1;
  v8int32 coeffs_rest = *(v8int32 *)weights_rest;
  v8int32 flux_out_coeff = *(v8int32 *)flux_out;

  v8int32 *ptr_out = (v8int32 *)out_flux1_base;
  v8int32 *restrict row0_ptr = (v8int32 *)row0_ptr_base;
  v8int32 *restrict row1_ptr = (v8int32 *)row1_ptr_base;
  v8int32 *restrict row2_ptr = (v8int32 *)row2_ptr_base;
  v8int32 *restrict row3_ptr = (v8int32 *)row3_ptr_base;
  v8int32 *restrict row4_ptr = (v8int32 *)row4_ptr_base;
  v8int32 *restrict r1;

  v16int32 data_buf1 = null_v16int32();
  v16int32 data_buf2 = null_v16int32();

  v8acc80 acc_0 = null_v8acc80();
  v8acc80 acc_1 = null_v8acc80();

  v8int32 lap_ij = null_v8int32();
  v8int32 lap_0 = null_v8int32();

  data_buf1 = upd_w(data_buf1, 0, *row3_ptr++);
  data_buf1 = upd_w(data_buf1, 1, *row3_ptr);
  data_buf2 = upd_w(data_buf2, 0, *row1_ptr++);
  data_buf2 = upd_w(data_buf2, 1, *row1_ptr);

  for (unsigned i = 0; i < COL / 8; i++)
    chess_prepare_for_pipelining chess_loop_range(1, ) {
      v16int32 flux_sub;

      acc_0 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_1 = lmul8(data_buf2, 1, 0x76543210, coeffs_rest, 0, 0x00000000);

      acc_0 = lmac8(acc_0, data_buf1, 2, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_1 = lmac8(acc_1, data_buf1, 1, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      row2_ptr = ((v8int32 *)row2_ptr_base) + i;
      data_buf2 = upd_w(data_buf2, 0, *(row2_ptr)++);
      data_buf2 = upd_w(data_buf2, 1, *(row2_ptr));

      acc_0 = lmac8(acc_0, data_buf2, 1, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 2, 0x76543210, coeffs, 0, 0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 3, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      lap_ij = srs(acc_0, 0);

      acc_1 = lmac8(acc_1, data_buf2, 0, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_1 = lmsc8(acc_1, data_buf2, 1, 0x76543210, coeffs, 0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf2, 2, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      lap_0 = srs(acc_1, 0);

      flux_sub =
          sub16(concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_0, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      ptr_out = (v8int32 *)out_flux1_base + i;
      *ptr_out = ext_w(flux_sub, 0);

      acc_0 = lmul8(data_buf1, 3, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmsc8(acc_0, data_buf2, 3, 0x76543210, coeffs, 0, 0x00000000);

      row1_ptr = ((v8int32 *)row1_ptr_base) + i;
      data_buf1 = upd_w(data_buf1, 0, *(row1_ptr)++);
      data_buf1 = upd_w(data_buf1, 1, *(row1_ptr));

      acc_0 = lmac8(acc_0, data_buf2, 2, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 4, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      acc_0 = lmac8(acc_0, data_buf1, 3, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      lap_0 = srs(acc_0, 0);

      flux_sub =
          sub16(concat(lap_0, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      ptr_out = (v8int32 *)out_flux2_base + i;
      *ptr_out = ext_w(flux_sub, 0);

      acc_1 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);
      acc_0 = lmul8(data_buf2, 2, 0x76543210, coeffs_rest, 0, 0x00000000);

      row0_ptr = ((v8int32 *)row0_ptr_base) + i;
      data_buf2 = upd_w(data_buf2, 0, *(row0_ptr)++);
      data_buf2 = upd_w(data_buf2, 1, *(row0_ptr));

      acc_1 = lmsc8(acc_1, data_buf1, 2, 0x76543210, coeffs, 0, 0x00000000);
      acc_1 = lmac8(acc_1, data_buf1, 1, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_1 = lmac8(acc_1, data_buf2, 2, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      row4_ptr = ((v8int32 *)row4_ptr_base) + i;
      data_buf2 = upd_w(data_buf2, 0, *(row4_ptr)++);
      data_buf2 = upd_w(data_buf2, 1, *(row4_ptr));

      acc_1 = lmac8(acc_1, data_buf1, 3, 0x76543210, coeffs_rest, 0,
                    0x00000000);
      acc_0 = lmac8(acc_0, data_buf2, 2, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      lap_0 = srs(acc_1, 0);

      flux_sub =
          sub16(concat(lap_ij, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98,
                concat(lap_0, undef_v8int32()), 0, 0x76543210, 0xFEDCBA98);
      ptr_out = (v8int32 *)out_flux3_base + i;
      *ptr_out = ext_w(flux_sub, 0);

      row3_ptr = ((v8int32 *)row3_ptr_base) + i;
      data_buf1 = upd_w(data_buf1, 0, *(row3_ptr)++);
      data_buf1 = upd_w(data_buf1, 1, *(row3_ptr));

      acc_0 = lmsc8(acc_0, data_buf1, 2, 0x76543210, coeffs, 0, 0x00000000);

      acc_0 = lmac8(acc_0, data_buf1, 1, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      row1_ptr = ((v8int32 *)row1_ptr_base) + i + 1;
      data_buf2 = upd_w(data_buf2, 0, *(row1_ptr)++);
      data_buf2 = upd_w(data_buf2, 1, *(row1_ptr));

      acc_0 = lmac8(acc_0, data_buf1, 3, 0x76543210, coeffs_rest, 0,
                    0x00000000);

      flux_sub = sub16(concat(srs(acc_0, 0), undef_v8int32()), 0, 0x76543210,
                       0xFEDCBA98, concat(lap_ij, undef_v8int32()), 0,
                       0x76543210, 0xFEDCBA98);
      ptr_out = (v8int32 *)out_flux4_base + i;
      *ptr_out = ext_w(flux_sub, 0);

      row3_ptr = ((v8int32 *)row3_ptr_base) + i + 1;
      data_buf1 = upd_w(data_buf1, 0, *(row3_ptr)++);
      data_buf1 = upd_w(data_buf1, 1, *(row3_ptr));
    }

  for (int i = 0; i < COL; ++i) window_writeincr(out_flux1_win, out_flux1_buf[i]);
  for (int i = 0; i < COL; ++i) window_writeincr(out_flux2_win, out_flux2_buf[i]);
  for (int i = 0; i < COL; ++i) window_writeincr(out_flux3_win, out_flux3_buf[i]);
  for (int i = 0; i < COL; ++i) window_writeincr(out_flux4_win, out_flux4_buf[i]);
}