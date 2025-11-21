
#pragma once
#include "../ImageSelect.hpp"
#include "../ComputeOp.hpp"
#include "SimpleImageOp.hpp"
class NegativeOp : public SimpleImageOp
{

    public:
    NegativeOp() :SimpleImageOp("Negative","src/shaders/Negative.glsl") {}
};