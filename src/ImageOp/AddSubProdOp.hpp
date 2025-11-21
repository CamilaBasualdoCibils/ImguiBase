#pragma once

#include "ImageOp.hpp"
#include "../ImageSelect.hpp"
#include "../ComputeOp.hpp"
enum Operations
{
    eAdd = 0,
    eSub = 1,
    eProd = 2
};
class AddSubProdOp : public ImageOp
{
    std::shared_ptr<Texture> Images[2];
    std::shared_ptr<Texture> OutputImage;
    ComputeOp computeOp;
    ImageSelect imageSelect1, imageSelect2;
    Operations currentOp = eAdd;

public:
    AddSubProdOp();
    void DrawView() override;
    void DrawGUI() override;
    void Run() override;
};