#pragma once
#include <adf.h>
#include <cstdint>

#include "ProcessUnit/include.h"
#include "ProcessUnit/hdiff.h"

class StencilCoreGraph : public adf::graph {
public:
    adf::port<input> row0_lap;
    adf::port<input> row1_lap;
    adf::port<input> row2_lap;
    adf::port<input> row3_lap;
    adf::port<input> row4_lap;

    adf::port<input> row1_flux;
    adf::port<input> row2_flux;
    adf::port<input> row3_flux;

    adf::port<output> out;

    adf::kernel k_lap;
    adf::kernel k_flux1;
    adf::kernel k_flux2;

    static constexpr int ROW_BYTES        = COL * sizeof(int32_t);
    static constexpr int FLUX_FWD_BYTES   = COL * sizeof(int32_t);
    static constexpr int FLUX_INTER_BYTES = 2 * COL * sizeof(int32_t);

    StencilCoreGraph() {
        using namespace adf;

        k_lap   = kernel::create(hdiff_lap);
        k_flux1 = kernel::create(hdiff_flux1);
        k_flux2 = kernel::create(hdiff_flux2);

        source(k_lap)   = "ProcessUnit/hdiff_lap.cc";
        source(k_flux1) = "ProcessUnit/hdiff_flux1.cc";
        source(k_flux2) = "ProcessUnit/hdiff_flux2.cc";

        headers(k_lap)   = {"ProcessUnit/include.h", "ProcessUnit/hdiff.h"};
        headers(k_flux1) = {"ProcessUnit/include.h", "ProcessUnit/hdiff.h"};
        headers(k_flux2) = {"ProcessUnit/include.h", "ProcessUnit/hdiff.h"};

        runtime<ratio>(k_lap)   = 0.9;
        runtime<ratio>(k_flux1) = 0.9;
        runtime<ratio>(k_flux2) = 0.9;

        location<kernel>(k_lap)   = tile(7, 1);
        location<kernel>(k_flux1) = tile(7, 2);
        location<kernel>(k_flux2) = tile(7, 3);

        connect<window<ROW_BYTES>>(row0_lap, k_lap.in[0]);
        connect<window<ROW_BYTES>>(row1_lap, k_lap.in[1]);
        connect<window<ROW_BYTES>>(row2_lap, k_lap.in[2]);
        connect<window<ROW_BYTES>>(row3_lap, k_lap.in[3]);
        connect<window<ROW_BYTES>>(row4_lap, k_lap.in[4]);

        connect<window<ROW_BYTES>>(row1_flux, k_flux1.in[0]);
        connect<window<ROW_BYTES>>(row2_flux, k_flux1.in[1]);
        connect<window<ROW_BYTES>>(row3_flux, k_flux1.in[2]);

        connect<window<FLUX_FWD_BYTES>>(k_lap.out[0], k_flux1.in[3]);
        connect<window<FLUX_FWD_BYTES>>(k_lap.out[1], k_flux1.in[4]);
        connect<window<FLUX_FWD_BYTES>>(k_lap.out[2], k_flux1.in[5]);
        connect<window<FLUX_FWD_BYTES>>(k_lap.out[3], k_flux1.in[6]);

        connect<window<FLUX_INTER_BYTES>>(k_flux1.out[0], k_flux2.in[0]);
        connect<window<FLUX_INTER_BYTES>>(k_flux1.out[1], k_flux2.in[1]);
        connect<window<FLUX_INTER_BYTES>>(k_flux1.out[2], k_flux2.in[2]);
        connect<window<FLUX_INTER_BYTES>>(k_flux1.out[3], k_flux2.in[3]);
        connect<window<FLUX_INTER_BYTES>>(k_flux1.out[4], k_flux2.in[4]);

        connect<window<ROW_BYTES>>(k_flux2.out[0], out);
    }
};