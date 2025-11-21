
#pragma once
#include "../ImageSelect.hpp"
#include "../ComputeOp.hpp"
#include "SimpleImageOp.hpp"
class LogTransformOp : public SimpleImageOp
{

    float C = 1.0f;
    void DrawGUI() override;
    public:
    LogTransformOp();
};