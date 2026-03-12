#include "TopGraph.h"

TopStencilGraph::TopStencilGraph()
    : in0(adf::input_gmio::create("hdiff_in0", 64, 1000)),
      in1(adf::input_gmio::create("hdiff_in1", 64, 1000)),
      in2(adf::input_gmio::create("hdiff_in2", 64, 1000)),
      in3(adf::input_gmio::create("hdiff_in3", 64, 1000)),
      in4(adf::input_gmio::create("hdiff_in4", 64, 1000)),
      out0(adf::output_gmio::create("hdiff_out0", 64, 1000)) {
    adf::connect<adf::window<COL * NBYTES>>(in0.out[0], core.in[0]);
    adf::connect<adf::window<COL * NBYTES>>(in1.out[0], core.in[1]);
    adf::connect<adf::window<COL * NBYTES>>(in2.out[0], core.in[2]);
    adf::connect<adf::window<COL * NBYTES>>(in3.out[0], core.in[3]);
    adf::connect<adf::window<COL * NBYTES>>(in4.out[0], core.in[4]);

    adf::connect<adf::window<COL * NBYTES>>(core.out, out0.in[0]);
}

TopStencilGraph topStencil;