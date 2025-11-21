#include "AddSubProdOp.hpp"
#include "PPMImage.hpp"
AddSubProdOp::AddSubProdOp() : ImageOp("Add/Sub/Product"), computeOp("src/shaders/AddSubProdOp.glsl")
{
}

void AddSubProdOp::DrawView()
{
    if (ImGui::BeginTabBar("ImageTabs"))
    {

        for (int i = 0; i < 3; i++)
        {
            std::string Title = std::string("Image") + std::to_string(i);
            if (i == 2)
                ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
            if (ImGui::BeginTabItem(i != 2 ? Title.c_str() : "Output", nullptr))
            {

                if (ImPlot::BeginPlot("PlotImage", ImGui::GetContentRegionAvail(), ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
                {
                    auto res = OutputImage->GetDimensions();
                    ImPlot::SetupAxisLimits(ImAxis_X1, 0, res.x);

                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, res.y);
                    auto image = i != 2 ? Images[i] : OutputImage;
                    ImPlot::PlotImage("InputImage", image->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(res.x, res.y));

                    ImPlot::EndPlot();
                }

                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

void AddSubProdOp::Run()
{
    computeOp.GetProgram().PushUniform<ivec2>("ImageSize",OutputImage->GetDimensions());
    computeOp.GetProgram().PushUniform<int>("uOp",currentOp);
    computeOp.Dispatch16_16_1(ivec3(OutputImage->GetDimensions(),1));
}

void AddSubProdOp::DrawGUI()
{
    ImGui::PushID(0);
    ImGui::TextUnformatted("Image 1");
    bool imagesModified = false;
    imagesModified = imageSelect1.DrawGUI();
    ImGui::PopID();
    ImGui::Separator();
    ImGui::TextUnformatted("Image 2");
    ImGui::PushID(1);

    imagesModified |= imageSelect2.DrawGUI();
    ImGui::PopID();
    ImGui::Separator();

    if (imagesModified)
    {
        const uvec2 TargetRes = glm::max(imageSelect1.GetSelected()->GetDimensions(), imageSelect2.GetSelected()->GetDimensions());
        Images[0] = imageSelect1.GetSelected()->BlitToNew(TargetRes);
        Images[1] = imageSelect2.GetSelected()->BlitToNew(TargetRes);
        OutputImage = std::make_shared<Texture>();
        OutputImage->Alloc2D(TargetRes);
        computeOp.SetInputTextures({Images[0], Images[1]});
        computeOp.SetOutputTextures({OutputImage});
    }
    if (ImGui::RadioButton("Add", currentOp == eAdd))
        currentOp = eAdd;
    if (ImGui::RadioButton("Substract", currentOp == eSub))
        currentOp = eSub;
    if (ImGui::RadioButton("eProduct", currentOp == eProd))
        currentOp = eProd;


      static std::string outputFilePath;
    ImGui::InputTextWithHint("##Path","Save Path",&outputFilePath);
    ImGui::SameLine();
    if (ImGui::Button("save")) PPMImage::SavePPMImage(OutputImage,outputFilePath);
}
