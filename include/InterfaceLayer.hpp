#pragma once
#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"
#include "GUIAuthorizationWindow.hpp"
#include "GUILobbiesListWindow.hpp"

class GUISettingsWindow;

class InterfaceLayer : public Engine::Layer, public ChessNetMessageListener,
					   public GUIAuthorizationWindowDelegate, public GUILobbiesListWindowDelegate,
					   public std::enable_shared_from_this<InterfaceLayer>
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	void OnAttach() override;
	void OnImGuiRender() override;

	void HandleNetMessage(const net::Message<ChessMessage>& msg) override;

	//Authorization Delegate
	void OnLogin(const std::string& user, const std::string& password) override;
	void OnRegister(const std::string& email, const std::string& user, const std::string& password) override;
	void OnLogout();
	//Lobby Delegate
	void OnLobbiesFetch() override;
    void OnLobbyCreate(std::string& name, std::string& password, int id) override;
    void OnLobbyJoin() override;
    void OnLobbySpectate() override;

private:
	std::vector<std::shared_ptr<Engine::GUIWindow>> windows;
	std::shared_ptr<GUISettingsWindow> settingsWindow;
	std::shared_ptr<GUIAuthorizationWindow> authorizationWindow;
	std::shared_ptr<GUILobbiesListWindow> lobbiesListWindow;

	Engine::GUIButton buttonGit;

	bool loginSuccessful = false;
	bool isLobbyMenuOpen = false;
	bool isChecked1 = false;

	User user;
	Lobby currentLobby;

};