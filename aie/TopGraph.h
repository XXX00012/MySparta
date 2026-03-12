#pragma once

#include <adf.h>
#include "./ProcessUnit/include.h"
#include "./ProcessGraph/StencilCoreGraph.h"

class TopStencilGraph : public adf::graph {
public:
    StencilCoreGraph core;

    adf::input_gmio in0;
    adf::input_gmio in1;
    adf::input_gmio in2;
    adf::input_gmio in3;
    adf::input_gmio in4;
    adf::output_gmio out0;

    TopStencilGraph();
};

extern TopStencilGraph topStencil;