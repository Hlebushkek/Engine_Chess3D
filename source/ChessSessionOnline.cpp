#include "ChessSessionOnline.hpp"
#include "ChessClient.hpp"
#include "Chess3D.hpp"

ChessSessionOnline::ChessSessionOnline(std::weak_ptr<ChessClient> client, std::weak_ptr<ChessBoard> board, const Lobby& lobby, const User& user)
    : ChessSession(board), m_client(client), m_lobby(lobby), m_user(user)
{
         if (m_user.id == m_lobby.user_white_id) this->m_playerType = PlayerType::White;
    else if (m_user.id == m_lobby.user_black_id) this->m_playerType = PlayerType::Black;
    else                                         this->m_playerType = PlayerType::Spectator;
}

ChessSessionOnline::~ChessSessionOnline()
{
    if (m_lobby.id)
        this->Surrender();
}

void ChessSessionOnline::Leave()
{
    if (auto client = m_client.lock())
        client->LeaveGame(m_user.id, m_lobby.id);

    m_lobby = {};
}

void ChessSessionOnline::Surrender()
{
    this->Leave();
}

void ChessSessionOnline::DidRequestMovePiece(glm::ivec2 from, glm::ivec2 to, PlayerType type)
{
    if (type != m_currentTurn || type != m_playerType)
        return;

    if (auto client = m_client.lock())
        client->MovePiece(from.x, from.y, to.x, to.y, m_user.id, m_lobby.id);
}

void ChessSessionOnline::HandleNetMessage(const net::Message<ChessMessage> &msg)
{
    switch (msg.header.id)
    {
    case ChessMessage::Moved:
    {
        glm::ivec2 from, to;
        auto msgCopy = msg;
        msgCopy >> to.y >> to.x >> from.y >> from.x;
        if (auto board = m_board.lock())
            board->MovePiece(from, to);
        NextTurn();
        break;
    }
    case ChessMessage::MoveDenied:
    {
        break;
    }
    default:
        break;
    }
}
