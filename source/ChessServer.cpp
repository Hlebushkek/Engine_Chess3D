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

			bool successful = db.Register(user);

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

			bool successful = db.LoginIn(user);

			net::Message<ChessMessage> responseMsg;
			if (successful)
			{
				responseMsg.header.id = ChessMessage::LoginAccepted;
				responseMsg << user;
			}
			else
			{
				responseMsg.header.id = ChessMessage::LoginDenied;
				responseMsg << "Login-in failed";
			}
			
			messageClient(client, responseMsg);
		}
		break;
	default:
		break;
	}
}
