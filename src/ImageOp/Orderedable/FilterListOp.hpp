#pragma once
#include "ImageOp/ImageOp.hpp"
#include "OrderOp.hpp"
#include "ImageSelect.hpp"
class FilterList : public ImageOp
{
    std::vector<std::shared_ptr<OrderOp>> ops;
    std::vector<std::shared_ptr<Texture>> OutputTextures;
    int SelectedIndex = -1;
    int DragSrcIndex = -1;
    ImageSelect InputImage;
    void Run() override;
    void DrawGUI() override;
    void DrawView() override;
    public:
    FilterList() : ImageOp("Filters")
    {

    };

};