#include "GaussianBlur.hpp"
void GaussianBlurOp::Run()
{
    const auto imgSize = GetInputTextures().front()->GetDimensions();
    Output->Alloc2D(imgSize, eRGBA, GL_RGBA8);
    Op.SetInputTextures(GetInputTextures());
    Op.SetOutputTextures({Output});
    Op.GetProgram().PushUniform<ivec2>("uSize", imgSize);
    Op.GetProgram().PushUniform<float>("uSigma", Sigma);
    Op.GetProgram().PushUniform<int>("uKernelSize", KernelSize);
    Op.GetProgram().PushUniform<int>("uBoundaryMode", boundary);
    // Dispatch16_16_1 1 thread per pixel (use local_size_x/y = 1 in the shader)
    Op.Dispatch16_16_1(uvec3(imgSize, 1));

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}

void GaussianBlurOp::DrawGUI()
{
    ImGui::DragFloat("Sigma", &Sigma);
    Sigma = glm::max(Sigma, 0.0001f);
    ImGui::DragInt("Kernel Size", &KernelSize);
    KernelSize = glm::max(KernelSize, 1);

    if (ImGui::RadioButton("Clamp", boundary == eClamp))
        boundary = eClamp;
    if (ImGui::RadioButton("Mirror", boundary == eMirror))
        boundary = eMirror;
    if (ImGui::RadioButton("Zero Pad", boundary == eZeroPad))
        boundary = eZeroPad;
}
