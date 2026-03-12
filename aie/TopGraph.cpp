#include "TopGraph.h"

TopStencilGraph::TopStencilGraph()
    : in0(input_gmio::create("hdiff_gmio_in0", 64, 1000)),
      in1(input_gmio::create("hdiff_gmio_in1", 64, 1000)),
      in2(input_gmio::create("hdiff_gmio_in2", 64, 1000)),
      in3(input_gmio::create("hdiff_gmio_in3", 64, 1000)),
      in4(input_gmio::create("hdiff_gmio_in4", 64, 1000)),
      out0(output_gmio::create("hdiff_gmio_out0", 64, 1000)) {
    connect(in0.out[0], core.in[0]);
    connect(in1.out[0], core.in[1]);
    connect(in2.out[0], core.in[2]);
    connect(in3.out[0], core.in[3]);
    connect(in4.out[0], core.in[4]);

    connect(core.out, out0.in[0]);
}

TopStencilGraph topStencil;