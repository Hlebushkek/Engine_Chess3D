#pragma once

#include "NetworkTypes.hpp"
#include "ChessSession.hpp"
#include "ChessNetEventDispatcher.hpp"

class ChessClient;
class ChessBoard;

class ChessSessionOnline : public ChessSession, public ChessNetMessageListener
{
public:
    ChessSessionOnline(std::weak_ptr<ChessClient> client, std::weak_ptr<ChessBoard> board, const Lobby& lobby, const User& user);
    ~ChessSessionOnline();

    void Leave() override;
    void Surrender() override;

    void DidRequestMovePiece(glm::ivec2 from, glm::ivec2 to, PlayerType type) override;

    void HandleNetMessage(const net::Message<ChessMessage>& msg) override;

private:
    std::weak_ptr<ChessClient> m_client;
    PlayerType m_playerType;
    Lobby m_lobby;
    User m_user;

};