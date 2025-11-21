#include "HistogramEqualize.hpp"
#include <array>
#include <algorithm>
void HistogramEqualizeOp::Run()
{
    // Image size
    const uvec2 imgSize = GetInputTextures().front()->GetDimensions();

    // SSBOs: 256 bins for histogram and 256-entry LUT/CDF
    HistSSBO = std::make_shared<SSBO>(sizeof(uint32) * 256);
    HistSSBO->FillWithZeros();
    lutSSBO = std::make_shared<SSBO>(sizeof(uint32) * 256);
    lutSSBO->FillWithZeros();
    // NOTE: make sure these are zeroed in your SSBO ctor or via a clear call.

    // --- Pass 1: Histogram
    HistogramOp.SetInputTextures(GetInputTextures());
    HistogramOp.SetOutputSSBOs({HistSSBO});
    HistogramOp.GetProgram().PushUniform<ivec2>("uSize", imgSize);
    // Dispatch16_16_1 1 thread per pixel (use local_size_x/y = 1 in the shader)
    HistogramOp.Dispatch16_16_1(uvec3(imgSize, 1));
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);

    // --- Pass 2: CDF + LUT build
    CDFOp.SetInputSSBOs({HistSSBO});
    CDFOp.SetOutputSSBOs({lutSSBO});
    CDFOp.GetProgram().PushUniform<uint32>("uTotalPixels", imgSize.x * imgSize.y);
    // Single workgroup of 256 threads (shader uses local_size_x = 256)
    CDFOp.Dispatch(uvec3(1, 1, 1));
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);

    // --- Pass 3: Apply LUT to produce output
    Output->Alloc2D(imgSize, eRGBA, GL_RGBA8);
    EqualizeOp.SetInputTextures(GetInputTextures());
    EqualizeOp.SetInputSSBOs({lutSSBO});
    EqualizeOp.SetOutputTextures({Output});
    EqualizeOp.GetProgram().PushUniform<ivec2>("uSize", imgSize);
    // Dispatch16_16_1 1 thread per pixel (local_size_x/y = 1 in the shader)
    EqualizeOp.Dispatch16_16_1(uvec3(imgSize, 1));
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

    RunOnce = true;
}

void ExportHistogram(const std::array<uint32_t, 256> &hist, const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char *>(hist.data()), hist.size() * sizeof(uint32_t));
    file.close();
}

void HistogramEqualizeOp::DrawGUI()
{
    if (!RunOnce)
    {
        return;
    }

    const auto FinalHistogram = Output->GetHistogram();
    const auto OriginalHistogram = GetInputTextures().front()->GetHistogram();

    if (ImGui::Button("save histograms"))
    {
        ExportHistogram(OriginalHistogram, "Original.bin");
        ExportHistogram(FinalHistogram, "Final.bin");
    }

    static float x[256];
    static float y1[256];
    static float y2[256];
    float MaxValue = 0;
    // Fill X axis (0..255)
    for (int i = 0; i < 256; ++i)
        x[i] = static_cast<float>(i);

    // Copy histogram data to floats

    for (int i = 0; i < 256; ++i)
    {
        y1[i] = static_cast<float>(OriginalHistogram[i]);
        y2[i] = static_cast<float>(FinalHistogram[i]);
        MaxValue = std::max(MaxValue, y1[i]);
        MaxValue = std::max(MaxValue, y2[i]);
    }

    if (ImPlot::BeginPlot("Histogram Comparison", ImVec2(-1, 300)))
    {
        ImPlot::SetupAxes("Pixel Value", "Frequency");
        ImPlot::SetupAxesLimits(0, 256, 0, MaxValue, ImPlotCond_Once);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 256);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, MaxValue);
        ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.2f, 0.7f, 1.0f, 1.0f)); // blue for original
        ImPlot::PlotLine("Original", x, y1, 256);
        ImPlot::PopStyleColor();

        ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 0.3f, 0.2f, 1.0f)); // red for equalized
        ImPlot::PlotLine("Equalized", x, y2, 256);
        ImPlot::PopStyleColor();

        ImPlot::EndPlot();
    }
}
