#pragma once

#include <net/NetServer.hpp>
#include "ChessDataBaseOperations.hpp"
#include "ChessGameOnline.hpp"
#include "NetworkTypes.hpp"

class ChessServer : public net::ServerInterface<ChessMessage>
{
public:
    ChessServer(uint16_t nPort);

protected:
    virtual bool onClientConnect(std::shared_ptr<net::Connection<ChessMessage>> client);
    virtual void onClientDisconnect(std::shared_ptr<net::Connection<ChessMessage>> client);
    virtual void onMessage(std::shared_ptr<net::Connection<ChessMessage>> client, net::Message<ChessMessage>& msg);

private:
    std::vector<std::shared_ptr<ChessGameOnline>> activeGames;
    std::unordered_map<int, std::shared_ptr<net::Connection<ChessMessage>>> userToConnection;

private:
    ChessDataBaseOperations *db = nullptr;

};