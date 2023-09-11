#include "ChessSessionOffline.hpp"

void ChessSessionOffline::Leave()
{

}

void ChessSessionOffline::Surrender()
{

}

void ChessSessionOffline::DidRequestMovePiece(glm::ivec2 from, glm::ivec2 to, PlayerType type)
{
    if (type != m_currentTurn)
        return;

    if (auto board = m_board.lock())
        board->MovePiece(from, to);

    NextTurn();
}
