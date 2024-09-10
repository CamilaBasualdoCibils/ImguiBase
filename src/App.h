#pragma once
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef _IMGUI_INC
#define _IMGUI_INC
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "implot.h"
#endif
class App
{
public:
    struct AppProperties
    {
        std::string AppName = "Default Name";
        uint32_t winSizeX =1280,winSizeY = 720;
        bool compatability_openGL_profile = true;
        bool imgui_docking_enable = false;
        bool imgui_viewports_enable = false;
        bool window_resizable = true;
        uint32_t GL_version_major = 3,GL_version_minor = 2; //this would be 3.1
    };

private:
    AppProperties properties;
    bool shouldShutdown = false;
    GLFWwindow* window = nullptr;
    ImGuiContext* imgui_context = nullptr;
    ImGuiIO* imgui_io = nullptr;
    ImPlotContext* implot_context = nullptr;
public:
    App(const AppProperties &_p);
    void Run();
    /// @brief Commands the app to shutdown when the frame is done rendering
    void Shutdown();

private:
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnStart() = 0;
    virtual void OnShutdown()= 0;
    void CleanUp();
    static void glfw_error_callback(int error, const char *description);
};