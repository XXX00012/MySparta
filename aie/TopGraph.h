#pragma once
#include <adf.h>
#include <string>
#include "./ProcessUnit/include.h"
#include "./ProcessGraph/StencilCoreGraph.h"

using namespace adf;

class TopStencilGraph : public graph {
public:
    StencilCoreGraph core;

    input_plio  in_plio[8];
    output_plio out_plio;

    TopStencilGraph(const std::string& graphID) {
        const std::string base = "./data/";

        for (int i = 0; i < 8; i++) {
            in_plio[i] = input_plio::create(
                graphID + "_in" + std::to_string(i),
                plio_32_bits,
                base + graphID + "_in" + std::to_string(i) + ".txt"
            );
        }

        out_plio = output_plio::create(
            graphID + "_out0",
            plio_32_bits,
            base + graphID + "_outputaie0.txt"
        );

        connect<window<COL * NBYTES>>(in_plio[0].out[0], core.in[0]);
        connect<window<COL * NBYTES>>(in_plio[1].out[0], core.in[1]);
        connect<window<COL * NBYTES>>(in_plio[2].out[0], core.in[2]);
        connect<window<COL * NBYTES>>(in_plio[3].out[0], core.in[3]);
        connect<window<COL * NBYTES>>(in_plio[4].out[0], core.in[4]);
        connect<window<COL * NBYTES>>(in_plio[5].out[0], core.in[5]);
        connect<window<COL * NBYTES>>(in_plio[6].out[0], core.in[6]);
        connect<window<COL * NBYTES>>(in_plio[7].out[0], core.in[7]);

        connect<window<COL * NBYTES>>(core.out, out_plio.in[0]);
    }
};