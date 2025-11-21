#include "OrderOp.hpp"

void OrderOp::DrawView()
{
}

void OrderOp::Entry(const std::vector<std::shared_ptr<Texture>> &_Inputtextures)
{
    InputTextures = _Inputtextures;
    Run();
}
