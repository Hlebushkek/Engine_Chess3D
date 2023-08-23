#pragma once

#include <Engine.hpp>
#include "ChessNetEventDispatcher.hpp"

class GUILobbiesListWindowDelegate
{
public:
    virtual void OnLobbiesFetch() = 0;
    virtual void OnLobbyCreate(std::string& name, std::string& password, int id) = 0;
    virtual void OnLobbyJoin(Lobby& lobby) = 0;
    virtual void OnLobbySpectate(int lobby_id) = 0;
};

class GUILobbiesListWindow : public Engine::GUIWindow, public ChessNetMessageListener
{
public:
    GUILobbiesListWindow();

    void InnerRender() override;

    void HandleNetMessage(const net::Message<ChessMessage>& msg) override;

public:
    std::weak_ptr<GUILobbiesListWindowDelegate> delegate;

private:
    std::vector<Lobby> lobbies;
    enum LobbyColumns { NAME = 0, USER_1, USER_2, SPECTATORS, ACTIONS };
};