
#pragma once
#include "ImageOp.hpp"
#include "../ImageSelect.hpp"
#include "../ComputeOp.hpp"
class SimpleImageOp : public ImageOp
{
    std::shared_ptr<Texture> InputImage;
    std::shared_ptr<Texture> OutputImage;
    ComputeOp computeOp;
    ImageSelect image1;
    
    protected:
    void DrawView()override;
    void DrawGUI()override;
    void Run() override;
    public:
    SimpleImageOp(const std::string& Name, const std::string& GLSLSourcePath);
    ComputeOp& GetComputeOp() {return computeOp;}

};