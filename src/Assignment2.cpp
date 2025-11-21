#include "Assignment2.hpp"
#include "App.h"
#include "Extra/Texture.h"
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <execution>
#include <filesystem>
#include <iostream>
#include "ImageOp/GammaOp.hpp"
#include "ImageOp/LogTransformOp.hpp"
#include "ImageOp/NegativeOp.hpp"
#include "ImageOp/ConnectedLabelingOp.hpp"
#include "ImageOp/AddSubProdOp.hpp"
#include "ImageOp/Orderedable/FilterListOp.hpp"
Assignemt2App::Assignemt2App() : App(AppProperties{
                                     .AppName = "Assignement 2",
                                     .compatability_openGL_profile = true,
                                     .imgui_docking_enable = false,
                                     .imgui_viewports_enable = false,
                                     .window_resizable = true,
                                     .GL_version_major = 4,
                                     .GL_version_minor = 3,
                                 })
{
    ImageOperations.push_back(std::make_shared<GammaOp>());
    ImageOperations.push_back(std::make_shared<LogTransformOp>());
    ImageOperations.push_back(std::make_shared<NegativeOp>());
    ImageOperations.push_back(std::make_shared<ConnectedLabelingOp>());
    ImageOperations.push_back(std::make_shared<AddSubProdOp>());
    ImageOperations.push_back(std::make_shared<FilterList>());
    SelectedOperation = ImageOperations.front();
}
std::shared_ptr<Texture> BeforeTexture, AfterTexture;
void Assignemt2App::OnStart()
{
}
void Assignemt2App::OnUpdate()
{
}
void Assignemt2App::OnRender()
{
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Main View", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);
    DrawImageView();
    ImGui::SameLine();
    DrawOptions();
    ImGui::End();
}
void Assignemt2App::OnPostRender()
{
}
void Assignemt2App::OnShutdown()
{
}

static bool Separate = true;
void Assignemt2App::DrawImageView()
{

    ImVec2 availableSpace = ImGui::GetContentRegionAvail();
    ImGui::SetNextWindowSizeConstraints(ImVec2(availableSpace.x * 0.2f, -1), ImVec2(availableSpace.x * 0.9, -1));

    if (ImGui::BeginChild("Image View", ImVec2(availableSpace.x * 0.8f, 0),ImGuiChildFlags_ResizeX))
    {
        SelectedOperation->Run();
       SelectedOperation->DrawView();
        ImGui::EndChild();
    }
}

void Assignemt2App::DrawOptions()
{
    if (ImGui::BeginChild("Options", ImVec2(0, 0), ImGuiChildFlags_Borders))
    {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::BeginCombo("##Files",SelectedOperation->GetName().data()))
        {
            int i = 0;
            for (const auto &imageop : ImageOperations)
            {
                const bool is_selected = (SelectedOperation == imageop);
                if (ImGui::Selectable(imageop->GetName().data(), is_selected))
                {
                    SelectedOperation = imageop;

                }
                
                // if ( ImGui::IsItemHovered())
                //     item_highlighted_idx = n;
                
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                ImGui::SetItemDefaultFocus();
                i++;
            }
            ImGui::EndCombo();
        }
        ImGui::Separator();
        
       if (ImGui::BeginChild("ImageOpOptions"))
       {
        SelectedOperation->DrawGUI();
        ImGui::EndChild();
       }
        
        ImGui::EndChild();
    }
}
