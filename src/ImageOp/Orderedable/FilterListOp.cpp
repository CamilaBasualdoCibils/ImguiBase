#include "FilterListOp.hpp"
#include "HistogramEqualize.hpp"
#include "SobelEdge.hpp"
#include "GaussianBlur.hpp"
#include "UnsharpMask.hpp"
#include "PPMImage.hpp"
void FilterList::Run()
{
    std::vector<std::shared_ptr<Texture>> InputTextures = {InputImage.GetSelected()};
    if (ops.empty())
    {
        OutputTextures = InputTextures;
    }
    else
    {
        for (const auto &filter : ops)
        {
            filter->Entry(InputTextures);
            InputTextures = filter->GetOutput();
        }
         OutputTextures = InputTextures;
    }
}

void FilterList::DrawGUI()
{
    InputImage.DrawGUI();
    ImGui::Text("Reorderable Operations:");
    ImGui::Separator();

      static std::string outputFilePath;
    ImGui::InputTextWithHint("##Path","Save Path",&outputFilePath);
    ImGui::SameLine();
    if (ImGui::Button("save")) PPMImage::SavePPMImage(OutputTextures.front(),outputFilePath);
    ImGui::Separator();
    // "+" button to add a new filter
    if (ImGui::Button("+"))
        ImGui::OpenPopup("AddFilterPopup");

    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Add a new filter from available types");

    // Dropdown popup for adding filters
    if (ImGui::BeginPopup("AddFilterPopup"))
    {
        // Example: these are possible filters
        const char *availableFilters[] = {"Histogram Equalize"};

        if (ImGui::MenuItem("Histogram Equalize"))
        {
            ops.push_back(std::make_shared<HistogramEqualizeOp>());
            SelectedIndex = (int)ops.size() - 1;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Sobel Edge"))
        {
            ops.push_back(std::make_shared<SobelEdgeOp>());
            SelectedIndex = (int)ops.size() - 1;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Gaussian Blur"))
        {
            ops.push_back(std::make_shared<GaussianBlurOp>());
            SelectedIndex = (int)ops.size() - 1;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Unsharp Mask"))
        {
            ops.push_back(std::make_shared<UnsharpMaskOp>());
            SelectedIndex = (int)ops.size() - 1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();

    for (int i = 0; i < (int)ops.size(); ++i)
    {
        ImGui::PushID(i);

        // Create a horizontal layout for filter + delete button
        ImGui::BeginGroup();
        if (ImGui::Selectable(ops[i]->GetName().data(), SelectedIndex == i, ImGuiSelectableFlags_AllowItemOverlap))
            SelectedIndex = i;

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20);

        if (ImGui::Button("-"))
        {
            ops.erase(ops.begin() + i);
            if (SelectedIndex == i)
                SelectedIndex = -1;
            else if (SelectedIndex > i)
                SelectedIndex--;
            ImGui::PopID();
            ImGui::EndGroup();
            break; // prevent iterator invalidation
        }

        // Drag & drop reordering
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            DragSrcIndex = i;
            ImGui::SetDragDropPayload("ORDER_OP_MOVE", &DragSrcIndex, sizeof(int));
            ImGui::Text("Move %s", ops[i]->GetName().data());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ORDER_OP_MOVE"))
            {
                int src = *(const int *)payload->Data;
                if (src != i)
                {
                    std::shared_ptr<OrderOp> temp = std::move(ops[src]);
                    if (src < i)
                        std::move(ops.begin() + src + 1, ops.begin() + i + 1, ops.begin() + src);
                    else
                        std::move_backward(ops.begin() + i, ops.begin() + src, ops.begin() + src + 1);
                    ops[i] = std::move(temp);
                    if (SelectedIndex == src)
                        SelectedIndex = i;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::EndGroup();
        ImGui::PopID();
    }


    int i = 0;
    for (const auto& op : ops)
    {
        ImGui::PushID(i++);
        ImGui::Separator();
        op->DrawGUI();
        ImGui::PopID();
    }
    /*
    // Details panel
    if (SelectedIndex >= 0 && SelectedIndex < (int)ops.size())
    {
        ImGui::Text("Selected: %s", ops[SelectedIndex]->GetName().data());
        ImGui::Separator();
        ops[SelectedIndex]->DrawGUI();
    }*/
}

void FilterList::DrawView()
{
    if (ImPlot::BeginSubplots("View", 1, 2, ImGui::GetContentRegionAvail(), ImPlotSubplotFlags_LinkAllX | ImPlotSubplotFlags_LinkAllY))
    {
        if (InputImage.GetSelected())
        {
            const auto &InputTex = InputImage.GetSelected();
            const auto AvailableSpace = ImGui::GetContentRegionAvail();
            const auto Half = ImVec2(AvailableSpace.x / 2, AvailableSpace.y);
            if (ImPlot::BeginPlot("Input", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto InputRes = InputTex->GetDimensions();
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, InputRes.x);

                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, InputRes.y);
                ImPlot::PlotImage("InputImage", InputTex->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(InputRes.x, InputRes.y));
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Output", Half, ImPlotFlags_CanvasOnly | ImPlotFlags_Equal))
            {
                const auto OutputRes = OutputTextures.front()->GetDimensions();
                ImPlot::PlotImage("OutputImage", OutputTextures.front()->GetHandle(), ImPlotPoint(0, 0), ImPlotPoint(OutputRes.x, OutputRes.y));
                ImPlot::EndPlot();
            }
        }

        ImPlot::EndSubplots();
    }
}
