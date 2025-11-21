#pragma once
#include "Extra/Texture.h"
#include <memory>
#include <stdint.h>
class ImageSelect
{
    int32_t indexSelected = -1;
    int32_t indexHovered = -1;
    std::shared_ptr<Texture> Hovered_Texture,Selected_Texture;

public:
    virtual bool DrawGUI();

    std::shared_ptr<Texture> GetSelected() const {return Selected_Texture;}
    std::shared_ptr<Texture> GetHovered() const {return Hovered_Texture;}
};