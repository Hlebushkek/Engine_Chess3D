#pragma once
#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"

class InterfaceLayer : public Engine::Layer, public ChessNetMessageListener
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	void FetchLobbies();

	virtual void OnImGuiRender() override;

	virtual void HandleNetMessage(const net::Message<ChessMessage>& msg) override;

private:
	Engine::Button buttonGit;
	bool loginSuccessful = false;
	bool isSettingsOpen = true; 
	bool isLogInOpen = true; 
	bool isLobbiesOpen = false; 
	bool isLobbyMenuOpen = false;
	bool isRegisterationOpen = false;
	bool isChecked1 = false;
	bool showPass = false;
	char buffer_user[32] = "";
	char buffer_pass[32] = "";
	char buffer_email[64] = "";
	ImVec2 lastClosedWindowPos;

	User user;

	std::vector<Lobby> lobbies;
	Lobby currentLobby;
	enum LobbyColumns { NAME = 0, USER_1, USER_2, SPECTATORS, ACTIONS };

};