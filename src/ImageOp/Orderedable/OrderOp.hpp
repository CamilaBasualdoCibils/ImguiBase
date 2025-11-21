#pragma once
#include "ImageOp/ImageOp.hpp"
#include "Extra/Texture.h"
class OrderOp : public ImageOp
{
    std::vector<std::shared_ptr<Texture>> InputTextures;


    void DrawView() override;

public:
    OrderOp(const std::string &Name) : ImageOp(Name) {};

    const auto &GetInputTextures() const { return InputTextures; }
    virtual void Entry(const std::vector<std::shared_ptr<Texture>> &Inputtextures);
    virtual std::vector<std::shared_ptr<Texture>> GetOutput() const { return InputTextures; }
};