#pragma once

#include <Engine.hpp>
#include "NetworkTypes.hpp"

class GUILobbyWindowDelegate
{
public:
    virtual void OnGameStart() = 0;
    virtual void OnLeave() = 0;
};

class GUILobbyWindow : public Engine::GUIWindow
{
public:
    GUILobbyWindow();
    void UpdateLobby(const Lobby& lobby);

    void InnerRender() override;

public:
    std::weak_ptr<GUILobbyWindowDelegate> delegate;

private:
    Engine::GUIButton startButton;

    Lobby m_lobby;

};