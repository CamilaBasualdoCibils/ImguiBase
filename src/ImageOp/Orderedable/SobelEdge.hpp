#pragma once
#include "OrderOp.hpp"
#include "ComputeOp.hpp"
#include "Extra/SSBO.hpp"
#include "Extra/Texture.h"
class SobelEdgeOp : public OrderOp
{
    ComputeOp Op;

    std::shared_ptr<Texture> Output = std::make_shared<Texture>();
    float Scale = 1.0f;
    void Run() override;

    void DrawGUI() override;

public:
   std::vector<std::shared_ptr<Texture>> GetOutput() const override { return {Output}; }

    SobelEdgeOp() : OrderOp("Sobel Edge Detect"), Op("src/shaders/SobelEdge.glsl"){}
};