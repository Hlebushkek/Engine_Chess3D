#pragma once

#include <cstdint>
#include <optional>
#include <net/NetMessage.hpp>

enum class ChessMessage : uint32_t
{
    Error,
    ServerPing,
    ServerMessage,
    MessageAll,
    Register,
    LoginIn,
    LoginAccepted,
    LoginDenied,
    LobbyCreate,
    LobbyJoin,
    LobbyJoined,
    LobbyJoinDenied,
    LobbyLeave,
    LobbyGet,
    LobbiesGet,
    GameStart,
    GameStarted,
    GameStartDenied,
    GameEnd,
    Move,
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
    std::optional<int> user_white_id;
    std::optional<int> user_black_id;
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

template <typename T>
struct SendableTrait<T, Lobby> {
    static void push(Message<T>& msg, const Lobby& lobby)
    {
        msg << lobby.id << lobby.name << lobby.password << lobby.user_white_id << lobby.user_black_id;
    }

    static void pull(Message<T>& msg, Lobby& lobby)
    {
        msg >> lobby.user_black_id >> lobby.user_white_id >> lobby.password >> lobby.name >> lobby.id;
    }
};

}