#include "ChessPieceObject.hpp"
#include "ChessBoard.hpp"

ChessPieceObject::ChessPieceObject(ChessPiece *pieceModel, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : GameObject(position, rotation, scale)
{
    this->pieceModel = pieceModel;
    this->meshes.push_back(pieceModel->GetMesh());
    this->textures.push_back(pieceModel->GetTexture());
    this->collider = new Engine::BoxCollider();
}

void ChessPieceObject::OnMouseDown(int button)
{
    if (auto parent = m_transform->GetParent().lock())
        if (auto board = std::dynamic_pointer_cast<ChessBoard>(parent->gameObject().lock()))
            board->UpdateSelection(this);
}