#pragma once
#include <adf.h>
#include <cstdint>

using namespace adf;

extern "C" {

void hdiff_lap(
    input_buffer<int32_t, extents<adf::inherited_extent>>& row0,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row1,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row2,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row3,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row4,
    output_buffer<int32_t, extents<COL>>& out_flux1,
    output_buffer<int32_t, extents<COL>>& out_flux2,
    output_buffer<int32_t, extents<COL>>& out_flux3,
    output_buffer<int32_t, extents<COL>>& out_flux4);

void hdiff_flux1(
    input_buffer<int32_t, extents<adf::inherited_extent>>& row1,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row2,
    input_buffer<int32_t, extents<adf::inherited_extent>>& row3,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_forward1,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_forward2,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_forward3,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_forward4,
    output_buffer<int32_t, extents<2 * COL>>& flux_inter1,
    output_buffer<int32_t, extents<2 * COL>>& flux_inter2,
    output_buffer<int32_t, extents<2 * COL>>& flux_inter3,
    output_buffer<int32_t, extents<2 * COL>>& flux_inter4,
    output_buffer<int32_t, extents<2 * COL>>& flux_inter5);

void hdiff_flux2(
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_inter1,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_inter2,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_inter3,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_inter4,
    input_buffer<int32_t, extents<adf::inherited_extent>>& flux_inter5,
    output_buffer<int32_t, extents<COL>>& out);

}