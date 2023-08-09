#pragma once

#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"
#include "ChessClient.hpp"

class Chess3D : public Engine::Application
{
public:
	Chess3D(const char* title, const int width, const int height);
	virtual ~Chess3D() = default;

	void Update() override;

	ChessClient& GetClient() { return client; }
	ChessNetMessageDispatcher& GetNetMessageDispatcher() { return netMsgDispatcher; }

private:
	void UpdateNetwork();

private:
	ChessClient client;
	ChessNetMessageDispatcher netMsgDispatcher;

};