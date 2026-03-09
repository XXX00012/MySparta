#pragma once
#include <adf.h>
#include <stdint.h>

void hdiff_lap(input_window_int32* row0,
               input_window_int32* row1,
               input_window_int32* row2,
               input_window_int32* row3,
               input_window_int32* row4,
               output_window_int32* out_flux1,
               output_window_int32* out_flux2,
               output_window_int32* out_flux3,
               output_window_int32* out_flux4);

void hdiff_flux1(input_window_int32* row1,
                 input_window_int32* row2,
                 input_window_int32* row3,
                 input_window_int32* flux_forward1,
                 input_window_int32* flux_forward2,
                 input_window_int32* flux_forward3,
                 input_window_int32* flux_forward4,
                 output_window_int32* flux_inter1,
                 output_window_int32* flux_inter2,
                 output_window_int32* flux_inter3,
                 output_window_int32* flux_inter4,
                 output_window_int32* flux_inter5);

void hdiff_flux2(input_window_int32* flux_inter1,
                 input_window_int32* flux_inter2,
                 input_window_int32* flux_inter3,
                 input_window_int32* flux_inter4,
                 input_window_int32* flux_inter5,
                 output_window_int32* out);