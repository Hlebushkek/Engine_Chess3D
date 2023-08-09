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
    ChessDataBaseOperations();

    std::vector<User> FetchUsers();

    bool Register(const User& user);
    bool LoginIn(const User& user);

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
}