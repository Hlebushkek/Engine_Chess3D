#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "ChessDataBaseOperations.hpp"

ChessDataBaseOperations::ChessDataBaseOperations()
    : sql(soci::mysql, "dbname=chess3d user=root password=jhtcnjdbx0509")
{
    CreateScheme();
    CreateTables();
}

std::vector<User> ChessDataBaseOperations::FetchUsers()
{
    std::vector<User> users; 

    try
    {
        soci::rowset<User> rs = (sql.prepare << "SELECT * FROM user");
        users.insert(users.end(), rs.begin(), rs.end());
        std::cout << "Found " << users.size() << " users. Example 0: " << users[0].name << " " << users[0].password << std::endl;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return users;
}

bool ChessDataBaseOperations::Register(const User& user)
{
    User fetchedUser;

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
            soci::into(fetchedUser);

        std::cout << "User registered successfully!" << std::endl;
        std::cout << "Id: " << fetchedUser.id << std::endl;

        return true;
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

bool ChessDataBaseOperations::LoginIn(const User& user)
{
    User fetchedUser;
    try
    {
        soci::statement st = (sql.prepare << "SELECT * FROM user WHERE name = :name AND password = :password",
            soci::use(user.name, "name"),
            soci::use(user.password, "password"),
            soci::into(fetchedUser));

        st.execute();
        if (st.fetch())
        {
            std::cout << "User login successfully! Id=" << fetchedUser.id << std::endl;
            return true;
        } else {
            std::cout << "User not found." << std::endl;
        }
    } catch (const soci::soci_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}

void ChessDataBaseOperations::executeSQLScript(std::string filename)
{
    std::string result;

    std::ifstream sqlFile("../resources/Database/" + filename);
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
