#pragma once

#include <vector>
#include <array>
#include "Engine.hpp"
#include "ChessTypes.hpp"
#include "ChessModel.hpp"

struct ChessBoardPieceObject;

class ChessBoard : public Engine::GameObject
{
public:
    ChessBoard();
    ~ChessBoard() = default;

    void UpdateSelection(ChessPiece* piece);
    void ResetSelection();

    void Render(Engine::Shader* shader) override;
    std::optional<Engine::Intersection> CollidesWith(const Ray& ray) override;

private:
    Engine::Texture *textureWhite;
    Engine::Texture *textureBlack;

    ChessModel model;

    std::vector<Engine::GameObject*> pieces;
    std::array<std::array<Engine::GameObject*, 8>, 8> boardBlocks;

};