#include "SobelEdge.hpp"

void SobelEdgeOp::Run()
{

    const auto imgSize = GetInputTextures().front()->GetDimensions();
    Output->Alloc2D(imgSize, eRGBA, GL_RGBA8);
    Op.SetInputTextures(GetInputTextures());
    Op.SetOutputTextures({Output});
    Op.GetProgram().PushUniform<ivec2>("uSize", imgSize);
    Op.GetProgram().PushUniform<float>("uScale", Scale);
    // Dispatch16_16_1 1 thread per pixel (use local_size_x/y = 1 in the shader)
    Op.Dispatch16_16_1(uvec3(imgSize, 1));

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}

void SobelEdgeOp::DrawGUI()
{
    ImGui::DragFloat("Scale", &Scale, 0.01f);
    
}
