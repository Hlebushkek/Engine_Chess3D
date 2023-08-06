#pragma once

#include <array>
#include "ChessTypes.hpp"
#include "ChessPieces.hpp"

class ChessModel
{
public:
    ChessModel();
    ~ChessModel() = default;

    std::vector<std::pair<glm::ivec2, MoveType>> GetSelectionFor(ChessPiece *piece);

    void MovePiece(glm::ivec2 from, glm::ivec2 to);

    ChessPiece *GetPieceAt(glm::ivec2 position);
    ChessPiece *GetPieceAt(int x, int y);

    bool PositionExists(glm::ivec2 position);
    bool PositionExists(int x, int y);

    glm::vec2 GetPositionFor(ChessPiece *piece);
private:
    std::array<std::array<ChessPiece*, 8>, 8> boardState;

};