#pragma once

#include <cstdint>
#include <net/NetMessage.hpp>

enum class ChessMessage : uint32_t
{
    ServerPing,
    MessageAll,
    ServerMessage,
    Register,
    LoginIn,
    LoginAccepted,
    LoginDenied,
    Move,
    LobbyCreate,
    LobbyJoin
};

struct User
{
    int id;
    std::string email;
    std::string name;
    std::string password;
};

struct Lobby
{
    int id;
    int user_white_id;
    int user_black_id;
    std::string name;
    std::string password;
};

struct Watcher
{
    int id;
    int user_id;
    int lobby_id;
};

struct GameHistory
{
    int id;
    int lobby_id;
    int user1_id;
    int user2_id;
    std::string moves;
};

namespace net
{

template <typename T>
struct SendableTrait<T, User> {
    static void push(Message<T>& msg, const User& user)
    {
        msg << user.id << user.email << user.name << user.password;
    }

    static void pull(Message<T>& msg, User& user)
    {
        msg >> user.password >> user.name >> user.email >> user.id;
    }
};

}