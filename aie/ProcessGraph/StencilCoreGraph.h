#pragma once
#include <adf.h>
#include "../ProcessUnit/include.h"
#include "../ProcessUnit/hdiff.h"

using namespace adf;

// in[0] = row0_lap
// in[1] = row1_lap
// in[2] = row2_lap
// in[3] = row3_lap
// in[4] = row4_lap
// in[5] = row1_flux
// in[6] = row2_flux
// in[7] = row3_flux

class StencilCoreGraph : public graph {
public:
    port<input>  in[8];
    port<output> out;

    kernel k_lap;
    kernel k_flux1;
    kernel k_flux2;

    StencilCoreGraph() {
        k_lap   = kernel::create(hdiff_lap);
        k_flux1 = kernel::create(hdiff_flux1);
        k_flux2 = kernel::create(hdiff_flux2);

        source(k_lap)   = "ProcessUnit/hdiff_lap.cc";
        source(k_flux1) = "ProcessUnit/hdiff_flux1.cc";
        source(k_flux2) = "ProcessUnit/hdiff_flux2.cc";

        headers(k_lap)   = {"ProcessUnit/hdiff.h", "include.h"};
        headers(k_flux1) = {"ProcessUnit/hdiff.h", "include.h"};
        headers(k_flux2) = {"ProcessUnit/hdiff.h", "include.h"};

        runtime<ratio>(k_lap)   = 0.9;
        runtime<ratio>(k_flux1) = 0.9;
        runtime<ratio>(k_flux2) = 0.9;

        location<kernel>(k_lap)   = tile(7, 1);
        location<kernel>(k_flux1) = tile(7, 2);
        location<kernel>(k_flux2) = tile(7, 3);

        connect<window<COL * NBYTES>>(in[0], k_lap.in[0]);
        connect<window<COL * NBYTES>>(in[1], k_lap.in[1]);
        connect<window<COL * NBYTES>>(in[2], k_lap.in[2]);
        connect<window<COL * NBYTES>>(in[3], k_lap.in[3]);
        connect<window<COL * NBYTES>>(in[4], k_lap.in[4]);

        connect<window<COL * NBYTES>>(in[5], k_flux1.in[0]);
        connect<window<COL * NBYTES>>(in[6], k_flux1.in[1]);
        connect<window<COL * NBYTES>>(in[7], k_flux1.in[2]);

        connect<window<COL * NBYTES>>(k_lap.out[0], k_flux1.in[3]);
        connect<window<COL * NBYTES>>(k_lap.out[1], k_flux1.in[4]);
        connect<window<COL * NBYTES>>(k_lap.out[2], k_flux1.in[5]);
        connect<window<COL * NBYTES>>(k_lap.out[3], k_flux1.in[6]);

        connect<window<2 * COL * NBYTES>>(k_flux1.out[0], k_flux2.in[0]);
        connect<window<2 * COL * NBYTES>>(k_flux1.out[1], k_flux2.in[1]);
        connect<window<2 * COL * NBYTES>>(k_flux1.out[2], k_flux2.in[2]);
        connect<window<2 * COL * NBYTES>>(k_flux1.out[3], k_flux2.in[3]);
        connect<window<2 * COL * NBYTES>>(k_flux1.out[4], k_flux2.in[4]);

        connect<window<COL * NBYTES>>(k_flux2.out[0], out);
    }
};