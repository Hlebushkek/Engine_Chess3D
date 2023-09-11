#pragma once

#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"
#include "ChessClient.hpp"

class ChessSession;
class ChessBoard;

class Chess3D : public Engine::Application
{
public:
	Chess3D(const char* title, const int width, const int height);
	virtual ~Chess3D() = default;

	void Update() override;

	ChessClient *GetClient() { return client.get(); }
	ChessNetMessageDispatcher& GetNetMessageDispatcher() { return netMsgDispatcher; }

private:
	void UpdateNetwork();

	void SetupSession(std::shared_ptr<ChessSession> session);

	void ApplicationWillTerminate() override;

private:
	User activeUser;
	Lobby activeLobby;

    std::shared_ptr<ChessBoard> board = nullptr;
	std::shared_ptr<ChessClient> client = nullptr;
	std::shared_ptr<ChessSession> activeSession = nullptr;

	ChessNetMessageDispatcher netMsgDispatcher;

};