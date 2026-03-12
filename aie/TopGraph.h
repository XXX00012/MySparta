#pragma once
#include <adf.h>
#include "./ProcessUnit/include.h"
#include "./ProcessGraph/StencilCoreGraph.h"

using namespace adf;

class TopStencilGraph : public graph {
public:
    StencilCoreGraph core;

    input_gmio in0;
    input_gmio in1;
    input_gmio in2;
    input_gmio in3;
    input_gmio in4;
    output_gmio out0;

    TopStencilGraph();
};

extern TopStencilGraph topStencil;