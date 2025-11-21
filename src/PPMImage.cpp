#include "PPMImage.hpp"
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>
#include <execution>
void PPMImage::SavePPMImage(std::shared_ptr<Texture> texture, std::string FilePath)
{
    const auto res = texture->GetDimensions();
    std::vector<uint8_t> Data(res.x * res.y * 4);
    texture->GetData(Data.data(), Data.size());

    uint32 CompCount = -1;
    if (texture->GetPixelFormat() == eR)
        CompCount = 1;
    else if (texture->GetPixelFormat() == eRG)
        CompCount = 2;
    else if (texture->GetPixelFormat() == eRGB)
        CompCount = 3;
    else if (texture->GetPixelFormat() == eRGBA)
        CompCount = 4;
    std::vector<unsigned char> rgb(res.x * res.y * 3);
    for (int i = 0; i < res.x * res.y; ++i)
    {
        unsigned char c[3];
        for (int j = 0; j < glm::min(CompCount, 3u);j++)
        {
            c[j] = Data[i * CompCount + j];
            rgb[i*3+j] = c[j];
        }
    }
    std::ofstream file(FilePath, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open " + FilePath);

    file << "P6\n"
         << res.x << " " << res.y << "\n255\n";
    file.write(reinterpret_cast<const char *>(rgb.data()), rgb.size());
    file.close();
}
std::shared_ptr<Texture> PPMImage::LoadPPMImage(const std::string &FilePath, std::function<Pixel(Pixel)> modifier)
{
    std::ifstream file(FilePath);

    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + FilePath);

    std::string Header = {};

    for (int i = 0; i < 4; i++)
    {
        std::string line = {};
        std::getline(file, line);
        if (i >= 2)

            Header += line + ' ';
    }
    std::stringstream header_read(Header);

    glm::ivec2 Resolution;
    uint32_t MaxColorValue;
    header_read >> Resolution.x;
    header_read >> Resolution.y;
    header_read >> MaxColorValue;

    std::string line = {};
    std::stringstream data_stream;
    data_stream << file.rdbuf();
    std::vector<Pixel> pixelData;
    pixelData.reserve(Resolution.x * Resolution.y);
    for (int y = 0; y < Resolution.y; y++)
    {
        for (int x = 0; x < Resolution.x; x++)
        {
            Pixel intColor;
            for (int i = 0; i < 3; i++)
            {
                uint32_t a;
                data_stream >> a;
                intColor[i] = a;
            }
            intColor.a = UINT8_MAX;
            pixelData.push_back(intColor);
        }
    }

    std::for_each(std::execution::par_unseq, pixelData.begin(), pixelData.end(), [modifier = modifier](Pixel &p)
                  { p = modifier(p); });
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->Alloc2D(Resolution, eRGBA, GL_RGBA8, pixelData.data());
    return texture;
}