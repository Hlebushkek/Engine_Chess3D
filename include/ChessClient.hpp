#pragma once

#include <net/NetClient.hpp>
#include "NetworkTypes.hpp"

class ChessClient : public net::ClientInterface<ChessMessage>
{
public:
    void PingServer();

    void Register(std::string login, std::string password);
    void LoginIn(std::string login, std::string password); 

private:

};