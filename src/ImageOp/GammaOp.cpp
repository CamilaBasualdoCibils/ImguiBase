#include "GammaOp.hpp"

void GammaOp::DrawGUI()
{
    SimpleImageOp::DrawGUI();
    ImGui::Separator();
    ImGui::SliderFloat("Gamma", &Gamma, -1, 10);

    GetComputeOp().GetProgram().PushUniform<float>("uGamma", Gamma);
}

GammaOp::GammaOp(): SimpleImageOp("Power (Gamma) Transform","src/shaders/PowerGammaTransform.glsl")
{

}
