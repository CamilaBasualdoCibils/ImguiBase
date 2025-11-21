#include "ImageSelect.hpp"
#include <filesystem>
#include <iostream>
#include "PPMImage.hpp"
std::vector<std::string> FilesInData()
{
    std::string path = "./data";
    size_t count = 0;
    std::vector<std::string> files;
    try
    {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
        {
            for (auto const &entry : std::filesystem::directory_iterator(path))
            {
                ++count;
                files.push_back(entry.path().filename().string());
            }
        }
        else
        {
            std::cerr << path << " does not exist or is not a directory\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return files;
}
bool ImageSelect::DrawGUI()
{
    bool Modified = false;
    auto DataItems = FilesInData();
      if (indexSelected == -1)
        {
            indexSelected = 0;
            Selected_Texture = PPMImage::LoadPPMImage("./data/" + DataItems[0]);
            Modified = true;
        }
        if (ImGui::BeginListBox("##Files",ImVec2(-1,0)))
        {
            int i = 0;
            for (const std::string &string : DataItems)
            {
                const bool is_selected = (indexSelected == i);
                if (ImGui::Selectable(string.c_str(), is_selected))
                {
                    indexSelected = i;
                    TextureFilter oldFilter = Selected_Texture->GetFilter();
                    Selected_Texture = PPMImage::LoadPPMImage("./data/" + string);
                    Selected_Texture->SetFilter(oldFilter);
                    Modified = true;
                }
                
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    if (indexHovered != i)
                    {
                        Hovered_Texture = PPMImage::LoadPPMImage("./data/"+string);
                        
                    }
                    ImGui::Image((ImTextureID) Hovered_Texture->GetHandle(),ImVec2(100,100));
                    ImGui::EndTooltip();
                }
                // if ( ImGui::IsItemHovered())
                //     item_highlighted_idx = n;
                
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                ImGui::SetItemDefaultFocus();
                i++;
            }
            ImGui::EndListBox();
        }
        return Modified;
}
