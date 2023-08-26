#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "ChessDataBaseOperations.hpp"
#include "config.h"

ChessDataBaseOperations::ChessDataBaseOperations(std::string schemeName, std::string login, std::string password)
    : sql(soci::mysql, "dbname=" + schemeName + " user=" + login + " password=" + password)
{
    CreateScheme();
    CreateTables();
}

bool ChessDataBaseOperations::Register(User& user)
{
    try
    {
        User douplicateUser;
        soci::statement st = (sql.prepare << "SELECT * FROM user WHERE name = :name",
            soci::use(user.name, "name"),
            soci::into(douplicateUser));

        st.execute();
        if (st.fetch())
        {
            std::cout << "User with this name already exist! Id=" << douplicateUser.id << std::endl;
            return false;
        }
        
        sql << "INSERT INTO user VALUES(:id, :email, :name, :password)",
            soci::use(0, "id"),
            soci::use(user.email, "email"),
            soci::use(user.name, "name"),
            soci::use(user.password, "password");

        sql << "SELECT * FROM user WHERE name = :name",
            soci::use(user.name, "name"),
            soci::into(user);

        std::cout << "User registered successfully!" << std::endl;
        std::cout << "Id: " << user.id << std::endl;

        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

bool ChessDataBaseOperations::LoginIn(User& user)
{
    try
    {
        soci::statement st = (sql.prepare << "SELECT * FROM user WHERE name = :name AND password = :password",
            soci::use(user.name, "name"),
            soci::use(user.password, "password"),
            soci::into(user));

        st.execute();
        if (st.fetch())
        {
            std::cout << "User login successfully! Id=" << user.id << std::endl;
            return true;
        } else {
            std::cout << "User not found." << std::endl;
        }
    } catch (const soci::soci_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

std::vector<User> ChessDataBaseOperations::FetchUsers()
{
    std::vector<User> users; 

    try
    {
        soci::rowset<User> rs = (sql.prepare << "SELECT * FROM user");
        users.insert(users.end(), rs.begin(), rs.end());
        std::cout << "Found " << users.size() << " users.\n";
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return users;
}

bool ChessDataBaseOperations::LobbyCreate(Lobby &lobby)
{
    try
    {
        sql << "INSERT INTO lobby VALUES(NULL, :user_white_id, NULL, :name, :password)",
            soci::use(lobby.user_white_id.value(), "user_white_id"),
            soci::use(lobby.name, "name"),
            soci::use(lobby.password, "password");

        sql << "SELECT * FROM lobby WHERE user_white_id = :user_white_id AND name = :name",
            soci::use(lobby.user_white_id.value_or(soci::i_null), "user_white_id"),
            soci::use(lobby.name, "name"),
            soci::into(lobby);

        std::cout << "Lobby registered successfully!" << std::endl;
        std::cout << "Id: " << lobby.id << std::endl;

        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

bool ChessDataBaseOperations::LobbyJoin(int user_id, Lobby& lobby)
{
    try
    {
        Lobby fetchedLobby;

        soci::statement st = (sql.prepare << "SELECT * FROM lobby WHERE id = :id",
            soci::use(lobby.id, "id"),
            soci::into(fetchedLobby));

        st.execute();
        if (st.fetch())
        {
            if (fetchedLobby.user_white_id != std::nullopt && fetchedLobby.user_black_id != std::nullopt)
            {
                std::cout << "Lobby is full." << std::endl;
                return false;
            }
            else if (fetchedLobby.user_white_id == std::nullopt && fetchedLobby.user_black_id == std::nullopt)
            {
                std::cout << "Lobby is empty." << std::endl;
                return false;
            }
            else if (fetchedLobby.user_white_id == lobby.user_black_id || fetchedLobby.user_black_id == lobby.user_white_id)
            {
                std::cout << "Other user change his side" << std::endl;
                std::optional<int> temp = lobby.user_black_id;
                lobby.user_black_id = lobby.user_white_id;
                lobby.user_white_id = temp;
            }

            if (lobby.user_white_id == std::nullopt)
                lobby.user_white_id = user_id;
            else if (lobby.user_black_id == std::nullopt)
                lobby.user_black_id = user_id;

            sql << "UPDATE lobby SET user_white_id = :user_white_id, user_black_id = :user_black_id WHERE id = :id",
                soci::use(lobby.user_white_id.value(), "user_white_id"),
                soci::use(lobby.user_black_id.value(), "user_black_id"),
                soci::use(lobby.id, "id"),
                soci::into(lobby);

            std::cout << "Joined successfully! Id=" << lobby.id << std::endl;
            return true;
        }
        else std::cout << "Lobby not found." << std::endl;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

bool ChessDataBaseOperations::LobbyGet(Lobby &lobby)
{
    try
    {
        sql << "SELECT * FROM lobby WHERE id = :id",
            soci::use(lobby.id, "id"),
            soci::into(lobby);

        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

std::vector<Lobby> ChessDataBaseOperations::FetchLobbies()
{
    std::vector<Lobby> lobbies; 

    try
    {
        soci::rowset<Lobby> rs = (sql.prepare << "SELECT * FROM lobby");
        lobbies.insert(lobbies.end(), rs.begin(), rs.end());
        std::cout << "Found " << lobbies.size() << " lobbies.\n";
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return lobbies;
}

bool ChessDataBaseOperations::LobbyLeave(int user_id, Lobby& lobby)
{
    try
    {
        Lobby fetchedLobby;

        sql << "SELECT * FROM lobby WHERE id = :id",
            soci::use(lobby.id, "id"),
            soci::into(fetchedLobby);

        if (fetchedLobby.user_white_id == user_id)
            sql << "UPDATE lobby SET user_white_id = NULL WHERE id = :id",
                soci::use(lobby.id, "id"),
                soci::into(fetchedLobby);
        else
            sql << "UPDATE lobby SET user_black_id = NULL WHERE id = :id",
                soci::use(lobby.id, "id"),
                soci::into(fetchedLobby);

        sql << "SELECT * FROM lobby WHERE id = :id",
            soci::use(lobby.id, "id"),
            soci::into(lobby);

        if (lobby.user_white_id == std::nullopt && lobby.user_black_id == std::nullopt)
            return LobbyDelete(lobby.id);

        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

bool ChessDataBaseOperations::LobbyDelete(int id)
{
    try
    {
        sql << "DELETE FROM lobby WHERE id = :id",
            soci::use(id, "id");
        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

void ChessDataBaseOperations::executeSQLScript(const std::string& filename)
{
    std::string result;

    std::string fullPath = RESOURCES_DIR + std::string("Database/") + filename;
    std::ifstream sqlFile(fullPath);
    if (!sqlFile.is_open())
    {
        std::cerr << "Failed to open the SQL file." << std::endl;
        return;
    }

    std::string sqlScript((std::istreambuf_iterator<char>(sqlFile)), std::istreambuf_iterator<char>());
    sqlFile.close();

    std::istringstream iss(sqlScript);
    std::string query;
    while (std::getline(iss, query, ';')) {
        boost::algorithm::trim(query);

        if (query.empty())
            continue;

        try {
            sql << query, soci::into(result);
            std::cout << "Querry response: " << result << std::endl;
        } catch (const soci::soci_error& e) {
            std::cerr << "Error executing query: " << e.what() << std::endl;
        }
    }
}

void ChessDataBaseOperations::CreateScheme()
{
    executeSQLScript("create_scheme.sql");
}

void ChessDataBaseOperations::CreateTables()
{
    executeSQLScript("create_user.sql");
    executeSQLScript("create_lobby.sql");
    executeSQLScript("create_spectator.sql");
}
