#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "ChessServer.hpp"

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
	boost::property_tree::ptree pt;
    boost::property_tree::read_json("../resources/configs/passwords.json", pt);

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
		}
		break;
	case ChessMessage::MessageAll:
		{
			std::cout << "[" << client->getID() << "]: Message All\n";

			net::Message<ChessMessage> msg;
			msg.header.id = ChessMessage::ServerMessage;
			msg << client->getID();
			messageAllClients(msg, client);
		}
		break;
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
				responseMsg << "Registeration failed";
			}
			
			messageClient(client, responseMsg);
		}
		break;
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
				responseMsg.header.id = ChessMessage::Error;
				responseMsg << "Login-in failed";
			}
			
			messageClient(client, responseMsg);
		}
		break;
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
		}
		break;
	case ChessMessage::LobbyJoin:
		{
			Lobby lobby;
			msg >> lobby;

			std::cout << "[" << client->getID() << "]: Join Lobby\n";
			std::cout << "Lobby id: " << lobby.id << ". " <<
						 "User w_id: "  << lobby.user_white_id.value_or(-1) << "; b_id: " << lobby.user_black_id.value_or(-1) << std::endl;

			bool successful = db->LobbyJoin(lobby);

			net::Message<ChessMessage> responseMsg;
			if (successful)
			{
				responseMsg.header.id = ChessMessage::LobbyJoined;
				responseMsg << lobby;
			}
			else
			{
				responseMsg.header.id = ChessMessage::LobbyJoinDenied;
				responseMsg << "Lobby join failed";
			}
			
			messageClient(client, responseMsg);
		}
		break;
	case ChessMessage::LobbyGet:
		{
			std::cout << "[" << client->getID() << "]: Get Lobbies\n";

			net::Message<ChessMessage> responseMsg;
			responseMsg.header.id = ChessMessage::LobbyGet;
			responseMsg << db->FetchLobbies();
			
			messageClient(client, responseMsg);
		}
		break;
	case ChessMessage::LobbyLeave:
		{
			std::cout << "[" << client->getID() << "]: Leave lobby\n";

			int user_id, lobby_id;
			msg >> lobby_id >> user_id;

			db->LobbyLeave(user_id, lobby_id);

			net::Message<ChessMessage> responseMsg;
			responseMsg.header.id = ChessMessage::LobbyLeave;

			messageClient(client, responseMsg);
		}
	default:
		break;
	}
}
