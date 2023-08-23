#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include "NetworkTypes.hpp"

class ChessDataBaseOperations
{
public:
    ChessDataBaseOperations(std::string schemeName, std::string login, std::string password);

    bool Register(User& user);
    bool LoginIn(User& user);
    std::vector<User> FetchUsers();

    bool LobbyCreate(Lobby& lobby);
    bool LobbyJoin(int user_id, Lobby& lobby);
    bool LobbyGet(Lobby& lobby);
    std::vector<Lobby> FetchLobbies();
    bool LobbyLeave(int user_id, Lobby& lobby);
    bool LobbyDelete(int id);

    void executeSQLScript(std::string filename);

private:
    soci::session sql;

    void CreateScheme();
    void CreateTables();

};

namespace soci
{
    template<>
    struct type_conversion<User>
    {
        typedef values base_type;

        static void from_base(values const& v, indicator ind, User& user)
        {
            user.id = v.get<int>("id");
            user.email = v.get<std::string>("email");
            user.name = v.get<std::string>("name");
            user.password = v.get<std::string>("password");
        }

        static void to_base(const User& user, values& v, indicator& ind)
        {
            v.set("id", user.id);
            v.set("email", user.email);
            v.set("name", user.name);
            v.set("password", user.password);
        }
    };

    template<>
    struct type_conversion<Lobby>
    {
        typedef values base_type;

        static void from_base(values const& v, indicator ind, Lobby& lobby)
        {
            lobby.id = v.get<int>("id");

            if (v.get_indicator("user_white_id") == soci::i_ok)
                 lobby.user_white_id = v.get<int>("user_white_id");
            else lobby.user_white_id = std::nullopt;

            if (v.get_indicator("user_black_id") == soci::i_ok)
                 lobby.user_black_id = v.get<int>("user_black_id");
            else lobby.user_black_id = std::nullopt;

            lobby.name = v.get<std::string>("name");
            lobby.password = v.get<std::string>("password");
        }

        static void to_base(const Lobby& lobby, values& v, indicator& ind)
        {
            v.set("id", lobby.id);

            if (lobby.user_white_id.has_value())
                 v.set("user_white_id", lobby.user_white_id.value());
            else ind = soci::i_null;

            if (lobby.user_black_id.has_value())
                 v.set("user_black_id", lobby.user_black_id.value());
            else ind = soci::i_null; //try value_or(i_null)

            v.set("name", lobby.name);
            v.set("password", lobby.password);
        }
    };
}