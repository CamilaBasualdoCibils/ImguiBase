#pragma once

#include "ImageOp.hpp"
#include "../ComputeOp.hpp"
#include "Extra/SSBO.hpp"
#include "Extra/Texture.h"
#include "ImageSelect.hpp"

enum ConnectedMode
{
    e4 = 0,
    e8 = 1,
    eM = 2
};
class ConnectedLabelingOp : public ImageOp
{
    ComputeOp InitPass, LoopPass,DrawPass;
    ImageSelect imageSelect;
    ConnectedMode connectedMode = e4;
    bool Capped = true;
    bool PauseRender = true;
    bool FirstRun = false;
    bool TransparentZero = true;
    bool RenderOnce = false;
    float Threshold = 0.5f;
    uint64 MaxIterations = 1000;
    uint64 RuntimeIterationCount = 0;
    std::shared_ptr<Texture> image;
    std::shared_ptr<Texture> outputColorImage;
    std::shared_ptr<SSBO> ChangedFlag;
    std::shared_ptr<Texture> LabelTextures[2];

public:
    ConnectedLabelingOp();
    void Run() override;
    void DrawView() override;
    void DrawGUI() override;
};