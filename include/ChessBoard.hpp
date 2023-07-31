#pragma once

#include <vector>
#include "Engine.hpp"
#include "ChessTypes.hpp"

class ChessBoard : public Engine::GameObject
{
public:
    ChessBoard();
    ~ChessBoard();

    void Render(Engine::Shader* shader) override;
    std::optional<Engine::Intersection> CollidesWith(const Ray& ray) override;

private:
    Engine::Texture *textureWhite;
    Engine::Texture *textureBlack;

    std::vector<Engine::GameObject*> pieces;
    std::vector<Engine::GameObject*> boardBlocks;
    std::vector<std::vector<OccupancyType>> boardState;

};