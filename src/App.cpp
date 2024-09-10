#include "App.h"
#include <iostream>
#include <exception>
App::App(const AppProperties &_p):properties(_p)
{

}

void App::Run()
{
    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("GLEW failed to init");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, properties.GL_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, properties.GL_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, properties.compatability_openGL_profile ? GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for OSX
    glfwWindowHint(GLFW_RESIZABLE, properties.window_resizable);
    glfwSetErrorCallback(glfw_error_callback);
    window = glfwCreateWindow(properties.winSizeX, properties.winSizeY, properties.AppName.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("GLEW failed to init");
    }
     const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "OpenGL version supported: " << version << std::endl;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui::StyleColorsDark();
    ImPlot::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable & properties.imgui_docking_enable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable  & properties.imgui_viewports_enable;   // Enable Multi-Viewport / Platform Windows

     OnStart();
    while (!shouldShutdown && !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //new imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        OnUpdate();
        OnRender();

        ImGui::EndFrame();
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
    }
OnShutdown();
    CleanUp();
}

void App::Shutdown()
{
    shouldShutdown = true;
}

void App::CleanUp()
{
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error " <<error <<": " << description << std::endl; 
}
