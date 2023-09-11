#pragma once

#include "ChessBoard.hpp"

class ChessSession : public ChessBoardDelegate, public std::enable_shared_from_this<ChessSession>
{
public:
    ChessSession(std::weak_ptr<ChessBoard> board) : m_currentTurn(PlayerType::White), m_board(board) {}

    virtual void Start()
    {
        if (auto board = m_board.lock())
        {
            board->delegate = weak_from_this();
            board->Reset();
        }
    }

    virtual void NextTurn() { m_currentTurn = (m_currentTurn == PlayerType::White) ? PlayerType::Black : PlayerType::White; }

    virtual void Leave() = 0;
    virtual void Surrender() = 0;

protected:
    PlayerType m_currentTurn;
    std::weak_ptr<ChessBoard> m_board;

    //Timer timer;

};