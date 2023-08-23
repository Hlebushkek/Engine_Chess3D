#pragma once

#include <array>
#include <optional>
#include "NetworkTypes.hpp"
#include "ChessTypes.hpp"

class ChessPiece;

class ChessSessionServer
{
public:
    ChessSessionServer(const Lobby& lobby);

    void Leave(int user_id);

    bool ValidateMove(int fromX, int fromY, int toX, int toY, int user_id);

    const Lobby& lobby() const { return m_lobby; }

private:
    PlayerType m_currentTurn = PlayerType::White;
    Lobby m_lobby;

    std::array<std::array<std::optional<PlayerType>, 8>, 8> board;

};