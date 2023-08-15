
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#elif __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

#include <string>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include "InterfaceLayer.hpp"
#include "Chess3D.hpp"
#include "Engine.hpp"

#ifdef _WIN32
void OpenURLWindows(const char* url)
{
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
#endif

#ifdef __APPLE__
void OpenURLMac(const char* url)
{
    CFStringRef urlString = CFStringCreateWithCString(kCFAllocatorDefault, url, kCFStringEncodingUTF8);
    CFURLRef urlRef = CFURLCreateWithString(kCFAllocatorDefault, urlString, NULL);

    if (urlRef)
    {
        LSOpenCFURLRef(urlRef, NULL);
        CFRelease(urlRef);
    }

    CFRelease(urlString);
}
#endif

ChessClient& client() { return (static_cast<Chess3D*>(Chess3D::Get()))->GetClient(); }

InterfaceLayer::InterfaceLayer()
    : Engine::Layer("InterfaceLayer"),
      buttonGit("Button", ImVec2(9, 10), ImVec2(90, 90), "../resources/github_icon.png", []() {
        const char* url = "https://github.com/Hlebushkek/Engine_Chess3D";
        #ifdef _WIN32
            OpenURLWindows(url);
        #elif __APPLE__
            OpenURLMac(url);
        #endif
    })
{
    (static_cast<Chess3D*>(Chess3D::Get()))->GetNetMessageDispatcher().AddListener(this);
}

void InterfaceLayer::FetchLobbies()
{
    client().FetchLobbies();
}

void InterfaceLayer::OnImGuiRender()
{
    ImGui::SetCurrentContext(Engine::ImGuiLayer::GetImguiContext());

    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Button] = ImVec4(0.7f, 0.7f, 0.7f, 0.1f);  
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);  
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f); 
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.7f); 
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (isSettingsOpen)
    {
        ImGui::Begin("Settings", &isSettingsOpen);
        
        static int counter = 0;

        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("CPU cache size: %d", SDL_GetCPUCacheLineSize());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        if (ImGui::Button("Counter button"))
        {
            std::cout << "counter button clicked\n";
            counter++;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::End();
    }

    style = ImGuiStyle();

    ImGui::SetNextWindowPos(ImVec2(50, ImGui::GetIO().DisplaySize.y - buttonGit.GetSize().y - 50));
    ImGuiWindowFlags windowFlags = 0;
    if (ImGui::Begin("Buttons", nullptr, windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |  ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysAutoResize )) // ImGuiWindowFlags_NoCollapse, ImGuiWindowFlags_NoMove and ImGuiWindowFlags_NoResize are not mandatory, it works without them. 
    {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        buttonGit.Render();
    }
    ImGui::End();
    
    style.Colors[ImGuiCol_Button] = ImVec4(0.7f, 0.7f, 0.7f, 0.1f);  
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);  
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f); 
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.7f); 
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    windowFlags = 0;
    if (isLogInOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(250,300));

        ImGui::Begin("Welcome to the club, buddy!", &isLogInOpen, windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("User: ");
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_user) / sizeof(char) - 1) + "##user").c_str(), buffer_user, sizeof(buffer_user));
        ImGui::Text("Password: ");
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_pass) / sizeof(char) - 1) + "##pass").c_str(), buffer_pass, sizeof(buffer_pass), !showPass ? ImGuiInputTextFlags_Password : 0);
        ImGui::Checkbox("Show Password", &showPass);

        if(ImGui::Button("Login"))
        {   
            std::cout << "Login: " << buffer_user << std::endl;
            std::cout << "Password: " << buffer_pass << std::endl;

            client().LoginIn(buffer_user, buffer_pass);
        }

        ImGui::SetCursorPos(ImVec2(8, ImGui::GetWindowHeight() - buttonGit.GetSize().y + 55));
        if(ImGui::Button("Create a new account"))
        {   
            lastClosedWindowPos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(lastClosedWindowPos);
            isRegisterationOpen = true;
            isLogInOpen = false;
        }

        ImGui::End();
    }

    if (isRegisterationOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(250,300));

        ImGui::Begin("Registration", &isLogInOpen, windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("Write your email: ");
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_email) / sizeof(char) - 1) + "##email").c_str(), buffer_email, sizeof(buffer_email));
        ImGui::Text("Create your login: ");
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_user) / sizeof(char) - 1) + "##user").c_str(), buffer_user, sizeof(buffer_user));
        ImGui::Text("Create your Password: ");
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_pass) / sizeof(char) - 1) + "##pass").c_str(), buffer_pass, sizeof(buffer_pass), !showPass ? ImGuiInputTextFlags_Password : 0);
        ImGui::Checkbox("Show Password", &showPass);

        if(ImGui::Button("Register "))
        {   
            client().Register(buffer_email, buffer_user, buffer_pass);
            isRegisterationOpen = true;
            isLogInOpen = false;
        }

        ImGui::SetCursorPos(ImVec2(8, ImGui::GetWindowHeight() - buttonGit.GetSize().y + 55));
        if(ImGui::Button("back to log in window"))
        {   
            lastClosedWindowPos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(lastClosedWindowPos);
            isLogInOpen = true;
            isRegisterationOpen = false;
        }
        ImGui::End();
    }

    style = ImGuiStyle();

    style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);  
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);   
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (isLobbiesOpen)
    {
        windowFlags = 0;
        ImGui::Begin("Lobby selection", &isLobbiesOpen);

        if (ImGui::Button("Create"))
        {
            client().CreateLobby("test1", "pass1", 1);
        }

        if (ImGui::Button("Refresh"))
        {
            FetchLobbies();
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
                    case LobbyColumns::NAME:
                        ImGui::Text(lobbies[row].name.c_str());
                        break;
                    case LobbyColumns::USER_1:
                        ImGui::Text(std::to_string(lobbies[row].user_white_id.value_or(-1)).c_str());
                        break;
                    case LobbyColumns::USER_2:
                        ImGui::Text(std::to_string(lobbies[row].user_black_id.value_or(-1)).c_str());
                        break;
                    case LobbyColumns::SPECTATORS:
                        ImGui::Text("Todo");
                        break;
                    case LobbyColumns::ACTIONS:
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
        ImGui::End();
    }

    style = ImGuiStyle();
    if (isLobbyMenuOpen)
    {
        ImGui::Begin("Lobby", &isLobbyMenuOpen);
        ImGui::Text("Name: %s", currentLobby.name.c_str());
        ImGui::Text("user_white_id: %d", currentLobby.user_white_id.value_or(-1));
        ImGui::Text("user_black_id: %d", currentLobby.user_black_id.value_or(-1));
        if (ImGui::Button("Leave"))
            client().LeaveLobby(user.id, currentLobby.id);
        ImGui::SameLine();
        ImGui::Button("Start");
        ImGui::End();
    }

    style = ImGuiStyle();

    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);        // Dark gray menu bar background
    style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);    // Hovered color for menu items
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);     // Active (pressed) color for menu items  
    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("System"))
        {
            ImGui::MenuItem("Log In", nullptr, &isLogInOpen, !loginSuccessful);
            ImGui::MenuItem("Lobby", nullptr, &isLobbiesOpen, loginSuccessful);
            ImGui::MenuItem("Settings", nullptr, &isSettingsOpen, true);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void InterfaceLayer::HandleNetMessage(const net::Message<ChessMessage> &msg)
{
    switch (msg.header.id)
    {
    case ChessMessage::LoginAccepted:
    {
        auto msgCopy = msg;
        msgCopy >> this->user;

        loginSuccessful = true;
        isLobbiesOpen = true;
        isLogInOpen = false;

        std::cout << "Login accepted" << std::endl;

        FetchLobbies();
        break;
    }
    case ChessMessage::LoginDenied:
    {
        loginSuccessful = false;
        std::cout << "Login denied" << std::endl;
        break;
    }
    case ChessMessage::LobbyGet:
    {
        std::cout << "Lobby get" << std::endl;
        auto msgCopy = msg;
        msgCopy >> this->lobbies;
        break;
    }
    case ChessMessage::LobbyJoined:
    {
        auto msgCopy = msg;
        Lobby lobby;
        msgCopy >> lobby;
        currentLobby = lobby;
        isLobbyMenuOpen = true;
        std::cout << "Lobby joined. " << lobby.name << ", " << lobby.password << std::endl;
        FetchLobbies();
        break;
    }
    case ChessMessage::LobbyJoinDenied:
    {
        auto msgCopy = msg;
        std::string text;
        msgCopy >> text;
        std::cout << text << std::endl;
        break;
    }
    case ChessMessage::LobbyLeave:
    {
        isLobbyMenuOpen = false;
        currentLobby = {};
        FetchLobbies();
        break;
    }
    default: break;
    }
}
