#include "ConnectedLabelingOp.hpp"
#include "PPMImage.hpp"
ConnectedLabelingOp::ConnectedLabelingOp() : ImageOp("Connected Labeling"), 
InitPass("src/shaders/ConnectedLabelingInit.glsl"), 
LoopPass("src/shaders/ConnectedLabelingLoop.glsl"),
DrawPass("src/shaders/ConnectedLabelingVis.glsl")
{
    ChangedFlag = std::make_shared<SSBO>(sizeof(GLuint));
}

void ConnectedLabelingOp::Run()
{
    if (!image || (FirstRun && PauseRender && !RenderOnce) )
        return;
    InitPass.SetInputTextures({image});
    InitPass.SetOutputTextures({LabelTextures[0]});
    InitPass.GetProgram().PushUniform<float>("Threshold",Threshold);
    LoopPass.GetProgram().PushUniform<float>("Threshold",Threshold);
    InitPass.Dispatch16_16_1(ivec3(image->GetDimensions(), 1));
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    int ping = 0;
    RuntimeIterationCount = 0;
    while (true)
    {
        LoopPass.GetProgram().Use();
        ChangedFlag->BindBase(3);
        GLuint flag = 0;
        ChangedFlag->Insert(&flag, sizeof(flag), 0);

        LoopPass.SetInputTextures({image, LabelTextures[(ping) % 2]});
        LoopPass.SetOutputTextures({LabelTextures[(ping + 1) % 2]});
        LoopPass.Dispatch16_16_1(ivec3(image->GetDimensions(), 1));
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                        GL_BUFFER_UPDATE_BARRIER_BIT);
        GLuint changed = 0;
        ChangedFlag->GetData(0, sizeof(changed), &changed);

        if (changed == 0)
        {
            break;
        }
        RuntimeIterationCount++;
        if (Capped && MaxIterations <= RuntimeIterationCount)
            break;
        ping ^= 1;
    }
    DrawPass.GetProgram().PushUniform<bool>("uZeroTransparent",TransparentZero);
    DrawPass.SetInputTextures({LabelTextures[(ping + 1) % 2]});
    DrawPass.SetOutputTextures({outputColorImage});
    DrawPass.Dispatch16_16_1(ivec3(image->GetDimensions(), 1));
    FirstRun = true;RenderOnce = false;
}

void ConnectedLabelingOp::DrawView()
{
     if (ImPlot::BeginSubplots("View", 1, 2, ImGui::GetContentRegionAvail(), ImPlotSubplotFlags_LinkAllX | ImPlotSubplotFlags_LinkAllY))
    {
        if (image)
        {
            const auto AvailableSpace = ImGui::GetContentRegionAvail();
            const auto Half = ImVec2(AvailableSpace.x / 2, AvailableSpace.y);
            if (ImPlot::BeginPlot("Input", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto InputRes = image->GetDimensions();
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, InputRes.x);

                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, InputRes.y);
                ImPlot::PlotImage("InputImage", image->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(InputRes.x, InputRes.y));
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Output", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto OutputRes = outputColorImage->GetDimensions();
                ImPlot::PlotImage("OutputImage", outputColorImage->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(OutputRes.x, OutputRes.y));
                ImPlot::EndPlot();
            }
        }

        ImPlot::EndSubplots();
    }
}

void ConnectedLabelingOp::DrawGUI()
{
    if (imageSelect.DrawGUI())
    {
        image = imageSelect.GetSelected();
        outputColorImage = std::make_shared<Texture>();
        outputColorImage->Alloc2D(image->GetDimensions(), eRGB, GL_RGBA8, nullptr);
        InitPass.GetProgram().PushUniform<ivec2>("ImageSize", image->GetDimensions());
        LoopPass.GetProgram().PushUniform<ivec2>("ImageSize", image->GetDimensions());
        DrawPass.GetProgram().PushUniform<ivec2>("ImageSize", image->GetDimensions());

        for (int i = 0; i < 2; i++)
        {
            LabelTextures[i] = std::make_shared<Texture>();
            LabelTextures[i]->Alloc2DStorage(image->GetDimensions(), GL_R32UI);
        }
    }
    ImGui::Separator();
    if (ImGui::RadioButton("4-Connected", connectedMode == e4))
        connectedMode = e4;
    if (ImGui::RadioButton("8-Connected", connectedMode == e8))
        connectedMode = e8;
    if (ImGui::RadioButton("M-Connected", connectedMode == eM))
        connectedMode = eM;
    ImGui::SliderFloat("Threshold",&Threshold,0.0f,1.0f);
    InitPass.GetProgram().PushUniform<int>("uConnMode", connectedMode);
    LoopPass.GetProgram().PushUniform<int>("uConnMode", connectedMode);
    if (ImGui::Button("Render")) RenderOnce = true;
    ImGui::Checkbox("Cap Iterations", &Capped);
    if (Capped)
    {
        uint64 Step = 10;
        ImGui::InputScalar("Max iteratiosn", ImGuiDataType_U64, &MaxIterations, &Step);
    }
    ImGui::Text("Done in %li iterations", RuntimeIterationCount);
    ImGui::Checkbox("Pause Render",&PauseRender);
    ImGui::Checkbox("Transparent null",&TransparentZero);
        if (ImGui::RadioButton("Nearest",image->GetFilter() == Nearest))
    {
        image->SetFilter(Nearest);
        outputColorImage->SetFilter(Nearest);
    }
    if (ImGui::RadioButton("Linear",image->GetFilter() == Linear))
    {
        image->SetFilter(Linear);
        outputColorImage->SetFilter(Linear);
    }

      static std::string outputFilePath;
    ImGui::InputTextWithHint("##Path","Save Path",&outputFilePath);
    ImGui::SameLine();
    if (ImGui::Button("save")) PPMImage::SavePPMImage(outputColorImage,outputFilePath);
}
