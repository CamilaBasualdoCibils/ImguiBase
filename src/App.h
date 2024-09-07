#pragma once
#include <string>
struct AppProperties
{
    std::string AppName;

};

class App
{

    AppProperties properties;
    bool shouldShutdown = false;
    public:
    App(const AppProperties &_p);
    void Run();
    /// @brief Commands the app to shutdown when the frame is done rendering
    void Shutdown();

private:
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    void CleanUp();
};