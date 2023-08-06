#pragma once

#include <Engine.hpp>
#include "ChessPieces.hpp"

class ChessPieceObject : public Engine::GameObject
{
public:
    ChessPieceObject(ChessPiece *pieceModel, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    virtual ~ChessPieceObject() = default;

    ChessPiece *GetModel() const { return pieceModel; }

    virtual void OnMouseDown(int button) override;

    GameObject *parent;
private:
    ChessPiece *pieceModel;
};