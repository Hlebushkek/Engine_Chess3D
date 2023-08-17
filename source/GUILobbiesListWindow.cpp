#include "GUILobbiesListWindow.hpp"
#include "Chess3D.hpp"

GUILobbiesListWindow::GUILobbiesListWindow()
    : GUIWindow("Lobby Selection", false)
{
    (static_cast<Chess3D*>(Chess3D::Get()))->GetNetMessageDispatcher().AddListener(this);
}

void GUILobbiesListWindow::InnerRender()
{
    if (ImGui::Button("Create"))
    {
        if (auto sharedDelegate = delegate.lock())
        {
            std::string lobbyName = "test1";
            std::string lobbyPass = "pass1";
            sharedDelegate->OnLobbyCreate(lobbyName, lobbyPass, 1);
        }
    }

    if (ImGui::Button("Refresh"))
    {
        if (auto sharedDelegate = delegate.lock())
            sharedDelegate->OnLobbiesFetch();
    }

    if (ImGui::BeginTable("Lobbies", 5, ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("White");
        ImGui::TableSetupColumn("Black");
        ImGui::TableSetupColumn("Spectators");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (size_t row = 0; row < lobbies.size(); row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 5; column++)
            {
                ImGui::TableSetColumnIndex(column);
                switch (column)
                {
                case NAME:
                    ImGui::Text(lobbies[row].name.c_str());
                    break;
                case USER_1:
                    ImGui::Text(std::to_string(lobbies[row].user_white_id.value_or(-1)).c_str());
                    break;
                case USER_2:
                    ImGui::Text(std::to_string(lobbies[row].user_black_id.value_or(-1)).c_str());
                    break;
                case SPECTATORS:
                    ImGui::Text("Todo");
                    break;
                case ACTIONS:
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.918f, 0.565f, 0.063f, 1.0f));
                    ImGui::Button("Join");
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.165f, 0.165f, 0.447f, 1.0f));
                    ImGui::Button("Spectate");
                    ImGui::PopStyleColor();
                    break;
                }
            }
        }
        ImGui::EndTable();
    }
}

void GUILobbiesListWindow::HandleNetMessage(const net::Message<ChessMessage> &msg)
{
    switch (msg.header.id)
    {
    case ChessMessage::LobbiesGet:
        {
            std::cout << "Lobby get" << std::endl;
            auto msgCopy = msg;
            msgCopy >> this->lobbies;
            break;
        }
    default:
        break;
    }
}
