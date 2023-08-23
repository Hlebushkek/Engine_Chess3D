#include "GUILobbyWindow.hpp"

GUILobbyWindow::GUILobbyWindow()
    : GUIWindow("Lobby", false), startButton("Start")
{
    startButton.enabled = false;
    startButton.SetOnClick([this]() {
        if (auto sharedDelegate = delegate.lock())
            sharedDelegate->OnGameStart();
    });
}

void GUILobbyWindow::UpdateLobby(const Lobby &lobby)
{
    this->m_lobby = lobby;
    startButton.enabled = lobby.user_white_id.has_value() && lobby.user_black_id.has_value();
}

void GUILobbyWindow::InnerRender()
{
    ImGui::Text("Name: %s", m_lobby.name.c_str());
    ImGui::Text("user_white_id: %d", m_lobby.user_white_id.value_or(-1));
    ImGui::Text("user_black_id: %d", m_lobby.user_black_id.value_or(-1));
    if (ImGui::Button("Leave"))
        if (auto sharedDelegate = delegate.lock())
            sharedDelegate->OnLeave();
    ImGui::SameLine();
    startButton.Render();
}