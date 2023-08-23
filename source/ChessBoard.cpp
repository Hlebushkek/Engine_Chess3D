#include <glm/gtx/string_cast.hpp>
#include "ChessBoard.hpp"
#include "ChessBoardPieceObject.hpp"
#include "ChessPieceObject.hpp"
#include "ChessPieces.hpp"
#include "Block.hpp"

ChessBoard::ChessBoard() : GameObject()
{
    textureWhite = Engine::Texture::LoadTexture("BlockB.png", GL_TEXTURE_2D);
    textureBlack = Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D);
}

void ChessBoard::UpdateSelection(ChessPieceObject *piece)
{
    ResetSelection();

    auto selections = model.GetSelectionFor(piece->GetModel());
    if (selections.size() == 0)
        return;

    selectedPiece = piece;

    for (auto selection : selections)
    {
        auto piece = std::static_pointer_cast<ChessBoardPieceObject>(this->boardBlocks[selection.first.y][selection.first.x]);
        piece->HighlightFor(selection.second);
    }
}

void ChessBoard::ResetSelection()
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            std::static_pointer_cast<ChessBoardPieceObject>(boardBlocks[y][x])->Reset();

    selectedPiece = nullptr;
}

void ChessBoard::MovePiece(const glm::ivec2 &from, const glm::vec2 &to)
{
    auto piece = GetPieceAt(from);
    if (piece == nullptr)
        return;

    if (ChessPiece *capturedPiece = model.GetPieceAt(to))
        RemovePiece(capturedPiece);

    model.MovePiece(from, to);
    piece->transform()->SetPosition(boardBlocks[to.y][to.x]->transform()->GetPosition() + glm::vec3(0.f, 0.03125f, 0.f));

    ResetSelection();
}

void ChessBoard::RequestMovePiece(ChessBoardPieceObject *piece)
{
    if (selectedPiece == nullptr)
        return;

    glm::ivec2 from = model.GetPositionFor(selectedPiece->GetModel());
    glm::ivec2 to = GetPositionFor(piece);

    if (from.x != INT_MAX && to.x != INT_MAX)
        if (delegate.lock())
            delegate.lock()->DidRequestMovePiece(from, to, selectedPiece->GetModel()->GetPlayer());
}

void ChessBoard::RemovePiece(ChessPiece *piece)
{
    for (auto it = pieces.begin(); it != pieces.end(); it++)
    {
        auto pieceObj = std::static_pointer_cast<ChessPieceObject>(*it);
        if (pieceObj->GetModel() == piece)
        {
            pieces.erase(it);
            break;
        }
    }
}

void ChessBoard::Reset()
{
    for (auto obj : pieces)
        obj->transform()->UnassignParent();
    pieces.clear();

    model = ChessModel();
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (ChessPiece *piece = model.GetPieceAt(x, y))
            {
                auto pieceObj = Engine::GameObject::Instantiate<ChessPieceObject>(piece, glm::vec3(0.5 + x * 0.0625, 0.03125, y * -0.0625), glm::vec3(0.f), glm::vec3(0.0625f));
                pieceObj->transform()->SetParent(m_transform);
                pieces.push_back(pieceObj);
            }
}

void ChessBoard::Initialize(std::shared_ptr<GameObject> object)
{
    GameObject::Initialize(object);

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            Engine::Texture *pieceTexture = ((x % 8 + y) % 2) == 0 ? textureWhite : textureBlack;
            auto piece = Engine::GameObject::Instantiate<ChessBoardPieceObject>(pieceTexture, glm::vec3(0.5 + x * 0.0625, 0, y * -0.0625), glm::vec3(0.f), glm::vec3(0.0625f));
            piece->transform()->SetParent(m_transform);
            boardBlocks[y][x] = piece;
        }
}

std::shared_ptr<ChessPieceObject> ChessBoard::GetPieceAt(const glm::ivec2 &position)
{
    auto pieceModel = model.GetPieceAt(position);

    for (auto obj : pieces)
    {
        auto pieceObj = std::static_pointer_cast<ChessPieceObject>(obj);
        if (pieceObj->GetModel() == pieceModel)
            return pieceObj;
    }

    return nullptr;
}

glm::ivec2 ChessBoard::GetPositionFor(ChessBoardPieceObject *piece)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (boardBlocks[y][x].get() == piece)
                return glm::vec2(x, y);

    return glm::vec2(INT_MAX);
}

std::optional<Engine::Intersection> ChessBoard::CollidesWith(const Ray &ray)
{
    std::optional<Engine::Intersection> result = std::nullopt;

    for (auto obj : pieces)
    {
        auto intersection = obj->CollidesWith(ray);
        if (intersection.has_value())
            if (result == std::nullopt || glm::distance(intersection.value().point, ray.origin) < glm::distance(result.value().point, ray.origin))
                result = intersection;
    }

    for (const auto& row : boardBlocks)
        for (auto obj : row)
        {
            auto intersection = obj->CollidesWith(ray);
            if (intersection.has_value())
                if (result == std::nullopt || glm::distance(intersection.value().point, ray.origin) < glm::distance(result.value().point, ray.origin))
                    result = intersection;
        }

    return result;
}
