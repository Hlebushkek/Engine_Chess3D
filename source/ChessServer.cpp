#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "config.h"
#include "ChessServer.hpp"
#include "ChessSessionServer.hpp"

int main()
{
    ChessServer server(60000); 
	server.start();

    while (true)
	{
		server.update(-1, true);
	}

    return 0;
}

ChessServer::ChessServer(uint16_t nPort)
	: net::ServerInterface<ChessMessage>(nPort)
{
    std::string configPath = RESOURCES_DIR + std::string("configs/passwords.json");
	boost::property_tree::ptree pt;
    boost::property_tree::read_json(configPath, pt);

    std::string login = pt.get<std::string>("db_credentials.login");
    std::string password = pt.get<std::string>("db_credentials.password");
	db = new ChessDataBaseOperations("chess3d", login, password);
}

bool ChessServer::onClientConnect(std::shared_ptr<net::Connection<ChessMessage>> client)
{
	net::Message<ChessMessage> msg;
	msg.header.id = ChessMessage::ServerMessage;
	client->send(msg);
	return true;
}

void ChessServer::onClientDisconnect(std::shared_ptr<net::Connection<ChessMessage>> client)
{
	std::cout << "Removing client [" << client->getID() << "]\n";
}

void ChessServer::onMessage(std::shared_ptr<net::Connection<ChessMessage>> client, net::Message<ChessMessage> &msg)
{
	std::cout << "Recieve Message with id=" << (int)msg.header.id << " from [" << client->getID() << "]\n";
	switch (msg.header.id)
	{
	case ChessMessage::ServerPing:
	{
		std::cout << "[" << client->getID() << "]: Server Ping\n";
		client->send(msg);
		break;
	}
	case ChessMessage::MessageAll:
	{
		std::cout << "[" << client->getID() << "]: Message All\n";

		net::Message<ChessMessage> msg;
		msg.header.id = ChessMessage::ServerMessage;
		msg << client->getID();
		messageAllClients(msg, client);
		break;
	}
	case ChessMessage::Register:
	{
		User user;
		msg >> user;

		std::cout << "Successfuly recieve Register message: " << user.name << " " << user.password << std::endl;

		bool successful = db->Register(user);

		net::Message<ChessMessage> responseMsg;
		if (successful)
		{
			responseMsg.header.id = ChessMessage::LoginAccepted;
			responseMsg << user;
		}
		else
		{
			responseMsg.header.id = ChessMessage::LoginDenied;
			responseMsg << std::string("Registeration failed");
		}

		client->setID(user.id);
		userConnections.insert_or_assign(user.id, client);
		messageClient(client, responseMsg);
		break;
	}
	case ChessMessage::LoginIn:
	{
		User user;
		msg >> user;

		std::cout << "[" << client->getID() << "]: Login In\n";
		std::cout << "Check credentials... " << std::endl;
		std::cout << "User: " << user.name << " " << user.password << std::endl;

		bool successful = db->LoginIn(user);

		net::Message<ChessMessage> responseMsg;
		if (successful)
		{
			responseMsg.header.id = ChessMessage::LoginAccepted;
			responseMsg << user;
		}
		else
		{
			responseMsg.header.id = ChessMessage::LoginDenied;
			responseMsg << std::string("Login-in failed.\nPlease, double-check\nentered name and password");
		}

		client->setID(user.id);
		userConnections.insert_or_assign(user.id, client);
		messageClient(client, responseMsg);
		break;
	}
	case ChessMessage::LobbyCreate:
	{
		Lobby lobby;
		msg >> lobby;

		std::cout << "[" << client->getID() << "]: Create Lobby\n";
		std::cout << "Lobby: " << lobby.name << " " << lobby.password << std::endl;

		bool successful = db->LobbyCreate(lobby);

		net::Message<ChessMessage> responseMsg;
		if (successful)
		{
			responseMsg.header.id = ChessMessage::LobbyJoined;
			responseMsg << lobby;
		}
		else
		{
			responseMsg.header.id = ChessMessage::LobbyJoinDenied;
			responseMsg << "Lobby creation failed";
		}
		
		messageClient(client, responseMsg);
		break;
	}
	case ChessMessage::LobbyJoin:
	{
		std::cout << "[" << client->getID() << "]: Join lobby\n";

		int user_id;
		Lobby lobby;
		msg >> lobby >> user_id;

		std::cout << "Lobby id: " << lobby.id << ". " <<
						"User w_id: "  << lobby.user_white_id.value_or(-1) << "; b_id: " << lobby.user_black_id.value_or(-1) << std::endl;

		bool successful = db->LobbyJoin(user_id, lobby);

		net::Message<ChessMessage> responseMsg;
		if (successful)
		{
			responseMsg.header.id = ChessMessage::LobbyJoined;
			responseMsg << lobby;
		}
		else
		{
			responseMsg.header.id = ChessMessage::LobbyJoinDenied;
			responseMsg << std::string("Lobby join failed");
		}
		
		messageClient(client, responseMsg);

		if (successful)
		{
			net::Message<ChessMessage> otherResponseMsg;
			otherResponseMsg.header.id = ChessMessage::LobbyUpdated;
			otherResponseMsg << lobby;
			if (lobby.user_white_id.has_value() && userConnections.at(lobby.user_white_id.value()) != client)
				messageClient(userConnections.at(lobby.user_white_id.value()), otherResponseMsg);
			if (lobby.user_black_id.has_value() && userConnections.at(lobby.user_black_id.value()) != client)
				messageClient(userConnections.at(lobby.user_black_id.value()), otherResponseMsg);
		}

		break;
	}
	case ChessMessage::LobbiesGet:
	{
		std::cout << "[" << client->getID() << "]: Get Lobbies\n";

		net::Message<ChessMessage> responseMsg;
		responseMsg.header.id = ChessMessage::LobbiesGet;
		responseMsg << db->FetchLobbies();
		
		messageClient(client, responseMsg);
		break;
	}
	case ChessMessage::LobbyLeave:
	{
		std::cout << "[" << client->getID() << "]: Leave lobby\n";

		int user_id;
		Lobby lobby;
		msg >> lobby >> user_id;

		bool successful = db->LobbyLeave(user_id, lobby);

		net::Message<ChessMessage> responseMsg;
		responseMsg.header.id = ChessMessage::LobbyLeave;

		messageClient(client, responseMsg);

		if (successful)
		{
			net::Message<ChessMessage> otherResponseMsg;
			otherResponseMsg.header.id = ChessMessage::LobbyUpdated;
			otherResponseMsg << lobby;
			if (lobby.user_white_id.has_value() && userConnections.at(lobby.user_white_id.value()) != client)
				messageClient(userConnections.at(lobby.user_white_id.value()), otherResponseMsg);
			if (lobby.user_black_id.has_value() && userConnections.at(lobby.user_black_id.value()) != client)
				messageClient(userConnections.at(lobby.user_black_id.value()), otherResponseMsg);
		}

		break;
	}
	case ChessMessage::GameStart:
	{
		std::cout << "[" << client->getID() << "]: Start lobby\n";

		int lobby_id;
		msg >> lobby_id;

		Lobby lobby{lobby_id};
		db->LobbyGet(lobby);

		bool isLobbyFull = lobby.user_white_id.has_value() && lobby.user_black_id.has_value();
		if (isLobbyFull)
			this->activeGames.emplace(lobby.id, std::make_shared<ChessSessionServer>(lobby));

		net::Message<ChessMessage> responseMsg;
		responseMsg.header.id = isLobbyFull ? ChessMessage::GameStarted : ChessMessage::GameStartDenied;

		if (lobby.user_white_id.has_value())
			messageClient(userConnections.at(lobby.user_white_id.value()), responseMsg);
		if (lobby.user_black_id.has_value())
			messageClient(userConnections.at(lobby.user_black_id.value()), responseMsg);

		break;
	}
	case ChessMessage::GameLeave:
	{
		std::cout << "[" << client->getID() << "]: Leave game\n";

		int user_id, lobby_id;
		msg >> lobby_id >> user_id;

		auto game = activeGames.at(lobby_id);
		if (!game) return;

		game->Leave(user_id);

		if (game->lobby().user_white_id == std::nullopt && game->lobby().user_black_id == std::nullopt)
			activeGames.erase(lobby_id);

		break;
	}
    case ChessMessage::GameEnd:
	{
		break;
	}
    case ChessMessage::Move:
	{
		std::cout << "[" << client->getID() << "]: Move\n";

		int user_id, lobby_id;
		int fromX, fromY, toX, toY;

		msg >> lobby_id >> user_id >> toY >> toX >> fromY >> fromX;

		auto game = activeGames.at(lobby_id);
		if (!game) return;

		bool isValidMove = game->ValidateMove(fromX, fromY, toX, toY, user_id);

		net::Message<ChessMessage> responseMsg;
		responseMsg.header.id = isValidMove ? ChessMessage::Moved : ChessMessage::MoveDenied;
		responseMsg << fromX << fromY << toX << toY;

		messageClient(userConnections.at(game->lobby().user_white_id.value()), responseMsg);
		messageClient(userConnections.at(game->lobby().user_black_id.value()), responseMsg);

		break;
	}
	default:
		break;
	}
}
