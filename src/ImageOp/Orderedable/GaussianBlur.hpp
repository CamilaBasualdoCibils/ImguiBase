#pragma once
#include "OrderOp.hpp"
#include "ComputeOp.hpp"
#include "Extra/SSBO.hpp"
#include "Extra/Texture.h"
class GaussianBlurOp : public OrderOp
{
    ComputeOp Op;

    std::shared_ptr<Texture> Output = std::make_shared<Texture>();
    float Sigma = 1.0f;
    int KernelSize =3;
    enum BoundaryMode // 0 = CLAMP, 1 = MIRROR, 2 = ZERO_PAD
    {
        eClamp = 0,
        eMirror = 1,
        eZeroPad = 2
    };
    BoundaryMode boundary = eClamp;
    void Run() override;

    void DrawGUI() override;

public:
   std::vector<std::shared_ptr<Texture>> GetOutput() const override { return {Output}; }

    GaussianBlurOp() : OrderOp("Gaussian Blur"), Op("src/shaders/GaussianBlur.glsl"){}
};