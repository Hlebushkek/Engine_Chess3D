#pragma once

#include <net/NetServer.hpp>
#include "ChessDataBaseOperations.hpp"
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
    ChessDataBaseOperations *db = nullptr;

};