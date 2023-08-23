#pragma once

#include "ChessSession.hpp"

class ChessBoard;

class ChessSessionOffline : public ChessSession
{
public:
    ChessSessionOffline(std::shared_ptr<ChessBoard> board) : ChessSession(board) {}

    void Leave() override;
    void Surrender() override;

    void DidRequestMovePiece(glm::ivec2 from, glm::ivec2 to, PlayerType type) override;

};