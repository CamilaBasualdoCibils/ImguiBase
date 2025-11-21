#include "UnsharpMask.hpp"

void UnsharpMaskOp::Run()
{
    const auto imgSize = GetInputTextures().front()->GetDimensions();
    Output->Alloc2D(imgSize, eRGBA, GL_RGBA8);
    Op.SetInputTextures(GetInputTextures());
    Op.SetOutputTextures({Output});
    Op.GetProgram().PushUniform<ivec2>("uSize", imgSize);
    Op.GetProgram().PushUniform<float>("uSigma", Sigma);
    Op.GetProgram().PushUniform<int>("uKernelSize", KernelSize);
    Op.GetProgram().PushUniform<float>("uStrength", Strength);
    Op.GetProgram().PushUniform<int>("uBoundaryMode", boundary);

    // Dispatch16_16_1 1 thread per pixel (use local_size_x/y = 1 in the shader)
    Op.Dispatch16_16_1(uvec3(imgSize, 1));

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}

void UnsharpMaskOp::DrawGUI()
{
    ImGui::DragFloat("Sigma", &Sigma, 0.1f, 0.1f, 10.0f);
    ImGui::DragInt("Kernel Size", &KernelSize, 1, 1, 25);
    if (KernelSize % 2 == 0)
        KernelSize++; // ensure odd
    ImGui::DragFloat("Strength", &Strength, 0.05f, 0.0f, 5.0f);

    if (ImGui::RadioButton("Clamp", boundary == eClamp))
        boundary = eClamp;
    if (ImGui::RadioButton("Mirror", boundary == eMirror))
        boundary = eMirror;
    if (ImGui::RadioButton("Zero Pad", boundary == eZeroPad))
        boundary = eZeroPad;
}
