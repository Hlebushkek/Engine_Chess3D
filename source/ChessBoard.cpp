#include <glm/gtx/string_cast.hpp>
#include "ChessBoard.hpp"
#include "ChessBoardPieceObject.hpp"
#include "ChessPieceObject.hpp"
#include "ChessPieces.hpp"
#include "Block.hpp"

ChessBoard::ChessBoard()
{
    textureWhite = Engine::Texture::LoadTexture("BlockB.png", GL_TEXTURE_2D);
    textureBlack = Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            Engine::Texture *pieceTexture = ((i % 8 + j) % 2) == 0 ? textureWhite : textureBlack;
            ChessBoardPieceObject *piece = new ChessBoardPieceObject(pieceTexture, glm::vec3(0.5 + i * 0.0625, 0, j * -0.0625), glm::vec3(0.f), glm::vec3(0.0625f));
            piece->parent = this;
            boardBlocks[i][j] = piece;
        }

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (ChessPiece *piece = model.GetPieceAt(x, y))
            {
                ChessPieceObject *pieceObj = new ChessPieceObject(piece, glm::vec3(0.5 + x * 0.0625, 0.03125, -y * 0.0625), glm::vec3(0.f), glm::vec3(0.0625f));
                pieceObj->parent = this;
                pieces.push_back(pieceObj);
            }
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
        ChessBoardPieceObject *piece = static_cast<ChessBoardPieceObject*>(this->boardBlocks[selection.first.x][selection.first.y]);
        piece->HighlightFor(selection.second);
    }
}

void ChessBoard::ResetSelection()
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            static_cast<ChessBoardPieceObject*>(boardBlocks[y][x])->Reset();

    selectedPiece = nullptr;
}

void ChessBoard::MovePieceTo(ChessBoardPieceObject *piece)
{
    if (selectedPiece == nullptr)
        return;

    glm::ivec2 from = model.GetPositionFor(selectedPiece->GetModel());
    glm::ivec2 to = GetPositionFor(piece);

    if (from.x != INT_MAX && to.x != INT_MAX)
    {
        if (ChessPiece *capturedPiece = model.GetPieceAt(to))
            RemovePiece(capturedPiece);

        model.MovePiece(from, to);
        selectedPiece->transform().SetPosition(piece->transform().GetPosition() + glm::vec3(0.f, 0.03125f, 0.f));

        ResetSelection();
    }
}

void ChessBoard::RemovePiece(ChessPiece *piece)
{
    for (auto it = pieces.begin(); it != pieces.end(); it++)
    {
        ChessPieceObject* pieceObj = static_cast<ChessPieceObject*>(*it);
        if (pieceObj->GetModel() == piece)
        {
            pieces.erase(it);
            break;
        }
    }
}

glm::ivec2 ChessBoard::GetPositionFor(ChessBoardPieceObject *piece)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (boardBlocks[y][x] == piece)
                return glm::vec2(y, x);

    return glm::vec2(INT_MAX);
}

void ChessBoard::Render(Engine::Shader *shader)
{
    GameObject::Render(shader);

    for (const auto& row : boardBlocks)
        for (auto obj : row)
            obj->Render(shader);

    for (auto obj : pieces)
        obj->Render(shader);
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
