#pragma once
#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"

class InterfaceLayer : public Engine::Layer, public ChessNetMessageListener
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	virtual void OnImGuiRender() override;

	virtual void HandleNetMessage(const net::Message<ChessMessage>& msg) override;

private:
	Engine::Button buttonGit;
	bool loginSuccessful = false;
	bool isSettingsOpen = true; 
	bool isLogInOpen = true; 
	bool isLobbiesOpen = false; 
	bool isChecked1 = false;
	bool showPass = false;
	char buffer_user[32] = "";
	char buffer_pass[32] = "";

};