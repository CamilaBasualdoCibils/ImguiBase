#include "LogTransformOp.hpp"

void LogTransformOp::DrawGUI()
{
    SimpleImageOp::DrawGUI();
    ImGui::SliderFloat("constant C",&C,0,3);
    GetComputeOp().GetProgram().PushUniform<float>("uC",C);
}

LogTransformOp::LogTransformOp() : SimpleImageOp("Log Transform", "src/shaders/LogTransform.glsl")
{
}