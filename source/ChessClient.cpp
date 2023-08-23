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

void ChessClient::Register(std::string email, std::string login, std::string password)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::Register;

    User user{0, email, login, password};
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
    msg << user_id << lobby;

    this->send(msg);
}

void ChessClient::SpectateLobby(int user_id, int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbySpectate;
    msg << user_id << lobby_id;

    this->send(msg);
}

void ChessClient::FetchLobbies()
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbiesGet;

    this->send(msg);
}

void ChessClient::LeaveLobby(int user_id, Lobby& lobby)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::LobbyLeave;

    msg << user_id << lobby;

    this->send(msg);
}

void ChessClient::StartGame(int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::GameStart;

    msg << lobby_id;

    this->send(msg);
}

void ChessClient::LeaveGame(int user_id, int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::GameLeave;

    msg << user_id << lobby_id;

    this->send(msg);
}

void ChessClient::MovePiece(int fromX, int fromY, int toX, int toY, int user_id, int lobby_id)
{
    net::Message<ChessMessage> msg;
    msg.header.id = ChessMessage::Move;

    msg << fromX << fromY << toX << toY << user_id << lobby_id;

    this->send(msg);
}