#pragma once
#include <adf.h>
#include <string>
#include "ProcessGraph/StencilCoreGraph.h"

using namespace adf;

template<int N>
class TopStencilGraph : public graph {
public:
    StencilCoreGraph<N> core;

    input_plio  in_plio[8];
    output_plio out_plio[N];

    TopStencilGraph(const std::string& graphID) {
        const std::string base = "./data/";

        for (int i = 0; i < 8; i++) {
            in_plio[i] = input_plio::create(
                graphID + "_in" + std::to_string(i),
                plio_32_bits,
                base + graphID + "_in" + std::to_string(i) + ".txt"
            );
        }

        for (int i = 0; i < N; i++) {
            out_plio[i] = output_plio::create(
                graphID + "_out" + std::to_string(i),
                plio_32_bits,
                base + graphID + "_outputaie" + std::to_string(i) + ".txt"
            );
        }

        for (int i = 0; i < 8; i++) {
            connect(in_plio[i].out[0], core.in[i]);
        }

        for (int i = 0; i < N; i++) {
            connect(core.out[i], out_plio[i].in[0]);
        }
    }
};