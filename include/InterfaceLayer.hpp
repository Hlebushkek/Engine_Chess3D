#pragma once
#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"
#include "GUIAuthorizationWindow.hpp"
#include "GUILobbiesListWindow.hpp"
#include "GUILobbyWindow.hpp"

class GUISettingsWindow;

class InterfaceLayer : public Engine::Layer, public ChessNetMessageListener,
					   public GUIAuthorizationWindowDelegate, public GUILobbiesListWindowDelegate, public GUILobbyWindowDelegate,
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
	void OnLogout() override;
	//LobbiesList Delegate
	void OnLobbiesFetch() override;
    void OnLobbyCreate(std::string& name, std::string& password) override;
    void OnLobbyJoin(Lobby& lobby) override;
    void OnLobbySpectate(int lobby_id) override;
	//Lobby Delegate
	void OnGameStart() override;
	void OnLeave() override;

private:
	std::vector<std::shared_ptr<Engine::GUIWindow>> windows;
	std::shared_ptr<GUISettingsWindow> settingsWindow;
	std::shared_ptr<GUIAuthorizationWindow> authorizationWindow;
	std::shared_ptr<GUILobbiesListWindow> lobbiesListWindow;
	std::shared_ptr<GUILobbyWindow> lobbyWindow;

	Engine::GUIButton buttonGit;

	bool loginSuccessful = false;
	bool gameInProgress = false;

	User user;
	Lobby currentLobby;

};