#pragma once
#include <string>

class ImageOp
{
std::string Name;
public:
ImageOp(const std::string& Name):Name(Name) {};
    virtual void DrawView() = 0;
    virtual void DrawGUI() = 0;

    virtual void Run() = 0;
    std::string_view GetName() const {return Name;}
};