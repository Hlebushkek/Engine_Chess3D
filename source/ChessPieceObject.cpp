#include "ChessPieceObject.hpp"
#include "ChessBoard.hpp"

ChessPieceObject::ChessPieceObject(ChessPiece *pieceModel, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : GameObject(position, rotation, scale)
{
    this->pieceModel = pieceModel;
    this->meshes.emplace_back(pieceModel->GetMesh());
    this->textures.emplace_back(pieceModel->GetTexture());
    this->collider = new Engine::BoxCollider();
}

void ChessPieceObject::OnMouseDown(int button)
{
    ChessBoard *board = (ChessBoard*)this->parent;
    board->UpdateSelection(this);
}