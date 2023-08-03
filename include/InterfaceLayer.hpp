#pragma once
#include <Engine.hpp>

class InterfaceLayer : public Engine::Layer
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	virtual void OnImGuiRender() override;

private:
	Engine::Button buttonGit;
	bool loginSuccessful = false;
	bool isSettingsOpen = true; 
	bool isLogInOpen = true; 
	bool isLobbiesOpen = false; 
	bool isChecked1 = false;
	bool showPass = false;
	char buffer_user[11] = "";
	char buffer_pass[11] = "";

};