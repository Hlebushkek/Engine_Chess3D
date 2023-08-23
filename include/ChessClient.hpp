#pragma once

#include <net/NetClient.hpp>
#include "NetworkTypes.hpp"
#include "ChessTypes.hpp"

class ChessClient : public net::ClientInterface<ChessMessage>
{
public:
    void PingServer();

    void Register(std::string email, std::string login, std::string password);
    void LoginIn(std::string login, std::string password);

    void CreateLobby(std::string name, std::string password, int user_id);
    void JoinLobby(int user_id, Lobby& lobby);
    void SpectateLobby(int user_id, int lobby_id);
    void FetchLobbies();
    void LeaveLobby(int user_id, Lobby& lobby);
    void StartGame(int lobby_id);
    void LeaveGame(int user_id, int lobby_id);

    void MovePiece(int fromX, int fromY, int toX, int toY, int user_id, int lobby_id);

private:

};