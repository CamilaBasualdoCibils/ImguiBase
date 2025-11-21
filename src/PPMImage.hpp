#pragma once
#include <memory>
#include "Extra/Texture.h"
#include <functional>
class PPMImage
{
public:
    using Pixel = vec<4, uint8_t>;
    static std::shared_ptr<Texture> LoadPPMImage(const std::string &FilePath, std::function<Pixel(Pixel)> modifier = [](Pixel v){return v;});

    static void SavePPMImage(std::shared_ptr<Texture> texture,std::string FilePath);
};