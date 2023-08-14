#include "ChessBoard.hpp"
#include "ChessBoardPieceObject.hpp"

ChessBoardPieceObject::ChessBoardPieceObject(Engine::Texture* baseTexture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : GameObject(position, rotation, scale)
{
    this->baseTexture = baseTexture;
    this->collider = new Engine::BoxCollider();

    Engine::Primitive cube = Engine::Cube();
    this->meshes.emplace_back(new Engine::Mesh(cube));

    Reset();
}

void ChessBoardPieceObject::OnMouseDown(int button)
{
    if (!isHighlighted)
        return;

    ChessBoard *board = (ChessBoard*)this->parent;
    board->MovePieceTo(this);
}

void ChessBoardPieceObject::HighlightFor(MoveType type)
{
    switch (type)
    {
    case MoveType::Move:
        RemoveTextureAt(0);
        // SetTexture(moveTexture, 0);
        break;
    case MoveType::Capture:
        RemoveTextureAt(0);
        // SetTexture(attackTexture, 0);
        break;
    }

    isHighlighted = true;
}

void ChessBoardPieceObject::Reset()
{
    SetTexture(baseTexture, 0);
    isHighlighted = false;
}
