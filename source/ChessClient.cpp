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

void ChessClient::CreateLobby(std::string name, std::string password, int user_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbyCreate;

    Lobby lobby{0, user_id, std::nullopt, name, password};
    msg << lobby;

    this->send(msg);
}

void ChessClient::JoinLobby(int user_id, Lobby& lobby)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbyJoin;

    if (lobby.user_white_id == std::nullopt)
        lobby.user_white_id = user_id;
    else if (lobby.user_black_id == std::nullopt)
        lobby.user_black_id = user_id;

    msg << lobby;

    this->send(msg);
}

void ChessClient::FetchLobbies()
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbiesGet;

    this->send(msg);
}

void ChessClient::LeaveLobby(int user_id, int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbyLeave;

    msg << user_id << lobby_id;

    this->send(msg);
}

void ChessClient::StartGame(int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::GameStart;

    msg << lobby_id;

    this->send(msg);
}