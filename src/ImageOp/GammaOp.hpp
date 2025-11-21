
#pragma once
#include "../ImageSelect.hpp"
#include "../ComputeOp.hpp"
#include "SimpleImageOp.hpp"
class GammaOp : public SimpleImageOp
{

    float Gamma = 1.0f;
    void DrawGUI()override;

    public:
    GammaOp();
};