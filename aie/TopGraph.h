#pragma once
#include <adf.h>
#include <string>
#include "./ProcessUnit/include.h"
#include "./ProcessGraph/StencilCoreGraph.h"

using namespace adf;

class TopStencilGraph : public graph {
public:
    StencilCoreGraph core;

    // DDR <-> AIE
    input_gmio  in_gmio[5];
    output_gmio out_gmio;

    TopStencilGraph(const std::string& graphID) {
        // 64-byte burst, 1000 MB/s request
        // 这里先不手工绑 shim 位置，让编译器自动放置
        for (int i = 0; i < 5; ++i) {
            in_gmio[i] = input_gmio::create(
                graphID + "_gmio_in" + std::to_string(i),
                64,
                1000
            );
        }

        out_gmio = output_gmio::create(
            graphID + "_gmio_out0",
            64,
            1000
        );

        // 5 rows -> core
        connect<window<COL * NBYTES>>(in_gmio[0].out[0], core.in[0]);
        connect<window<COL * NBYTES>>(in_gmio[1].out[0], core.in[1]);
        connect<window<COL * NBYTES>>(in_gmio[2].out[0], core.in[2]);
        connect<window<COL * NBYTES>>(in_gmio[3].out[0], core.in[3]);
        connect<window<COL * NBYTES>>(in_gmio[4].out[0], core.in[4]);

        connect<window<COL * NBYTES>>(core.out, out_gmio.in[0]);
    }
};