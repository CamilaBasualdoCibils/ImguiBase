#pragma once
#include <array>
#include "OrderOp.hpp"
#include "ComputeOp.hpp"
#include "Extra/SSBO.hpp"
#include "Extra/Texture.h"
class HistogramEqualizeOp : public OrderOp
{
    std::shared_ptr<SSBO> HistSSBO, lutSSBO;
    ComputeOp HistogramOp, CDFOp, EqualizeOp;
    bool RunOnce = false;
    std::shared_ptr<Texture> Output = std::make_shared<Texture>();
    void Run() override;

    void DrawGUI() override;

public:
   std::vector<std::shared_ptr<Texture>> GetOutput() const override { return {Output}; }

    HistogramEqualizeOp() : OrderOp("Histogram Equalize"), HistogramOp("src/shaders/Histogram/Histogram.glsl"), CDFOp("src/shaders/Histogram/CDF.glsl"), EqualizeOp("src/shaders/Histogram/Equalize.glsl") {}
};