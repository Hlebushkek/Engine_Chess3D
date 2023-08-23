#pragma once

#include <vector>
#include <array>
#include "Engine.hpp"
#include "ChessTypes.hpp"
#include "ChessModel.hpp"

class ChessBoardPieceObject;
class ChessPieceObject;

class ChessBoardDelegate
{
public:
    virtual void DidRequestMovePiece(glm::ivec2 from, glm::ivec2 to, PlayerType type) = 0;
};

class ChessBoard : public Engine::GameObject
{
public:
    ChessBoard();
    ~ChessBoard() = default;

    void UpdateSelection(ChessPieceObject *piece);
    void ResetSelection();

    void MovePiece(const glm::ivec2& from, const glm::vec2& to);
    void RequestMovePiece(ChessBoardPieceObject *piece);

    void RemovePiece(ChessPiece *pieceModel);

    void Reset();

    void Initialize(std::shared_ptr<GameObject> object) override;

    std::shared_ptr<ChessPieceObject> GetPieceAt(const glm::ivec2& position);
    glm::ivec2 GetPositionFor(ChessBoardPieceObject *piece);

    std::optional<Engine::Intersection> CollidesWith(const Ray& ray) override;

    std::weak_ptr<ChessBoardDelegate> delegate;

private:
    Engine::Texture *textureWhite;
    Engine::Texture *textureBlack;

    ChessModel model;

    std::vector<std::shared_ptr<Engine::GameObject>> pieces;
    std::array<std::array<std::shared_ptr<Engine::GameObject>, 8>, 8> boardBlocks;

    ChessPieceObject* selectedPiece = nullptr;

};