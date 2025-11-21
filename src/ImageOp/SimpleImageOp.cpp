#include "SimpleImageOp.hpp"
#include "PPMImage.hpp"
void SimpleImageOp::DrawView()
{
    if (ImPlot::BeginSubplots("View", 1, 2, ImGui::GetContentRegionAvail(), ImPlotSubplotFlags_LinkAllX | ImPlotSubplotFlags_LinkAllY))
    {
        if (InputImage)
        {
            const auto AvailableSpace = ImGui::GetContentRegionAvail();
            const auto Half = ImVec2(AvailableSpace.x / 2, AvailableSpace.y);
            if (ImPlot::BeginPlot("Input", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto InputRes = InputImage->GetDimensions();
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, InputRes.x);

                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, InputRes.y);
                ImPlot::PlotImage("InputImage", InputImage->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(InputRes.x, InputRes.y));
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Output", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto OutputRes = OutputImage->GetDimensions();
                ImPlot::PlotImage("OutputImage", OutputImage->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(OutputRes.x, OutputRes.y));
                ImPlot::EndPlot();
            }
        }

        ImPlot::EndSubplots();
    }
}

void SimpleImageOp::DrawGUI()
{
    if (image1.DrawGUI())
    {
        InputImage = image1.GetSelected();
        computeOp.SetInputTextures({InputImage});

        OutputImage->Alloc2D(InputImage->GetDimensions(), InputImage->GetPixelFormat(), InputImage->GetInternalFormat(), nullptr);
        computeOp.SetOutputTextures({OutputImage});
    }
    if (ImGui::RadioButton("Nearest",InputImage->GetFilter() == Nearest))
    {
        InputImage->SetFilter(Nearest);
        OutputImage->SetFilter(Nearest);
    }
    if (ImGui::RadioButton("Linear",InputImage->GetFilter() == Linear))
    {
        InputImage->SetFilter(Linear);
        OutputImage->SetFilter(Linear);
    }

    static std::string outputFilePath;
    ImGui::InputTextWithHint("##Path","Save Path",&outputFilePath);
    ImGui::SameLine();
    if (ImGui::Button("save")) PPMImage::SavePPMImage(OutputImage,outputFilePath);

}

void SimpleImageOp::Run()
{
    if (InputImage)
    {

        computeOp.Dispatch16_16_1(uvec3(InputImage->GetDimensions(), 1));
    }
}

SimpleImageOp::SimpleImageOp(const std::string &Name, const std::string &GLSLSourcePath) : ImageOp(Name), computeOp(GLSLSourcePath)
{
    OutputImage = std::make_shared<Texture>();
}
