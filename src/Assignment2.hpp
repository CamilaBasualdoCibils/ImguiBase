#pragma once
#include "App.h"
#include <memory>
#include "Extra/Texture.h"
#include "ImageOp/ImageOp.hpp"
class Assignemt2App: public App
{
    std::vector<std::shared_ptr<ImageOp>> ImageOperations;
    std::shared_ptr<ImageOp> SelectedOperation;
    public:
    Assignemt2App();
    private:
    void OnStart() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnPostRender() override;
    void OnShutdown() override;
    void DrawImageView();
    void DrawOptions();

};