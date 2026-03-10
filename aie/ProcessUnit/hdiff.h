#pragma once
#include <adf.h>

using namespace adf;

extern "C" {

void hdiff_lap(input_window_int32* row0_win,
               input_window_int32* row1_win,
               input_window_int32* row2_win,
               input_window_int32* row3_win,
               input_window_int32* row4_win,
               output_window_int32* out_flux1_win,
               output_window_int32* out_flux2_win,
               output_window_int32* out_flux3_win,
               output_window_int32* out_flux4_win);

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
                 output_window_int32* flux_inter5_win);

void hdiff_flux2(input_window_int32* flux_inter1_win,
                 input_window_int32* flux_inter2_win,
                 input_window_int32* flux_inter3_win,
                 input_window_int32* flux_inter4_win,
                 input_window_int32* flux_inter5_win,
                 output_window_int32* out_win);

}