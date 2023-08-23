#include "ChessSessionServer.hpp"
#include "ChessPieces.hpp"

ChessSessionServer::ChessSessionServer(const Lobby& lobby)
    : m_lobby(lobby)
{
    for (size_t i = 0; i < 8; i++)
    {
        this->board[0][i] = PlayerType::White;
        this->board[1][i] = PlayerType::White;
        this->board[6][i] = PlayerType::Black;
        this->board[7][i] = PlayerType::Black;
    }
}

void ChessSessionServer::Leave(int user_id)
{
    if (user_id == this->m_lobby.user_white_id)
        this->m_lobby.user_white_id = std::nullopt;
    else if (user_id == this->m_lobby.user_black_id)
        this->m_lobby.user_black_id = std::nullopt;
}

bool ChessSessionServer::ValidateMove(int fromX, int fromY, int toX, int toY, int user_id)
{
    bool isValidMove =
        this->board[fromY][fromX] &&
        this->board[fromY][fromX].value() == m_currentTurn &&
        (!this->board[toY][toX] || this->board[toY][toX].value() != m_currentTurn);

    if (isValidMove)
    {
        this->board[toY][toX] = this->board[fromY][fromX];
        this->board[fromY][fromX] = std::nullopt;
        this->m_currentTurn = this->m_currentTurn == PlayerType::White ? PlayerType::Black : PlayerType::White;
    }

    return isValidMove;
}