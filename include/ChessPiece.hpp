#pragma once

#include <Engine.hpp>
#include "Pieces.hpp"

class ChessPiece : public Engine::GameObject
{
public:
    ChessPiece(Piece *pieceModel, glm::vec3 position);
    virtual ~ChessPiece();

    void Render(Engine::Shader* shader) override;

private:
    Engine::Texture *texture;
};