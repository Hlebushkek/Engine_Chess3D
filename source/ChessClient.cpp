#include "ChessClient.hpp"
#include "NetworkTypes.hpp"

void ChessClient::PingServer()
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::ServerPing;
    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();		
    msg << timeNow;

    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(timeNow.time_since_epoch());
    std::cout << "try ping server with msg: " << duration.count() << std::endl;
    
    this->send(msg);
}

void ChessClient::Register(std::string login, std::string password)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::Register;

    User user{0, login, login, password};
    msg << user;

    this->send(msg);
}

void ChessClient::LoginIn(std::string login, std::string password)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LoginIn;	

    User user{0, login, login, password};
    msg << user;

    this->send(msg);
}