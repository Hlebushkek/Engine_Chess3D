
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

#include <string>
#include <imgui.h>
#include <Engine.hpp>
#include "InterfaceLayer.hpp"
#include "GUISettingsWindow.hpp"
#include "GUIAuthorizationWindow.hpp"
#include "Chess3D.hpp"

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
    : Engine::Layer("InterfaceLayer"), buttonGit("##gitButton", ImVec2(9, 10), ImVec2(90, 90), "../resources/github_icon.png")
{
    (static_cast<Chess3D*>(Chess3D::Get()))->GetNetMessageDispatcher().AddListener(this);

    buttonGit.SetOnClick([]() {
        const char* url = "https://github.com/Hlebushkek/Engine_Chess3D";
        #ifdef _WIN32
            OpenURLWindows(url);
        #elif __APPLE__
            OpenURLMac(url);
        #endif
    });

    settingsWindow = std::make_shared<GUISettingsWindow>();
    authorizationWindow = std::make_shared<GUIAuthorizationWindow>();
    lobbiesListWindow = std::make_shared<GUILobbiesListWindow>();
    this->windows.push_back(settingsWindow);
    this->windows.push_back(authorizationWindow);
    this->windows.push_back(lobbiesListWindow);
}

void InterfaceLayer::OnAttach()
{
    authorizationWindow->delegate = weak_from_this();
    lobbiesListWindow->delegate = weak_from_this();
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

    for (auto window : windows)
        window->Render();
    
    ImGui::SetNextWindowPos(ImVec2(50, ImGui::GetIO().DisplaySize.y - buttonGit.GetSize().y - 50));
    ImGuiWindowFlags windowFlags = 0;
    if (ImGui::Begin("Buttons", nullptr, windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |  ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysAutoResize )) // ImGuiWindowFlags_NoCollapse, ImGuiWindowFlags_NoMove and ImGuiWindowFlags_NoResize are not mandatory, it works without them. 
    {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        buttonGit.Render();
    }
    ImGui::End();

    if (isLobbyMenuOpen)
    {
        ImGui::Begin("Lobby", &isLobbyMenuOpen);
        ImGui::Text("Name: %s", currentLobby.name.c_str());
        ImGui::Text("user_white_id: %d", currentLobby.user_white_id.value_or(-1));
        ImGui::Text("user_black_id: %d", currentLobby.user_black_id.value_or(-1));
        if (ImGui::Button("Leave"))
            client().LeaveLobby(user.id, currentLobby.id);
        ImGui::SameLine();
        if (ImGui::Button("Start"))
            client().StartGame(currentLobby.id);
        ImGui::End();
    }

    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);        // Dark gray menu bar background
    style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);    // Hovered color for menu items
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);     // Active (pressed) color for menu items  
    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("System"))
        {
            ImGui::MenuItem("Authorization", nullptr, &authorizationWindow->isVisible, true);
            ImGui::MenuItem("Lobby", nullptr, &lobbiesListWindow->isVisible, loginSuccessful);
            ImGui::MenuItem("Settings", nullptr, &settingsWindow->isVisible, true);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void InterfaceLayer::OnLogin(const std::string& user, const std::string& password)
{
    client().LoginIn(user, password);
}

void InterfaceLayer::OnRegister(const std::string &email, const std::string &user, const std::string &password)
{
    client().Register(email, user, password);
}

void InterfaceLayer::OnLogout()
{
    isLobbyMenuOpen = false;
    lobbiesListWindow->isVisible = false;
    currentLobby = {};
    loginSuccessful = false;
    user = {};
}

void InterfaceLayer::OnLobbiesFetch()
{   
    client().FetchLobbies();
}

void InterfaceLayer::OnLobbyCreate(std::string &name, std::string &password, int id)
{
    client().CreateLobby(name, password, id);
}

void InterfaceLayer::OnLobbyJoin()
{

}

void InterfaceLayer::OnLobbySpectate()
{

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
        lobbiesListWindow->isVisible = true;

        authorizationWindow->SetAuthorizationSuccess();
        authorizationWindow->isVisible = false;

        client().FetchLobbies();
        break;
    }
    case ChessMessage::LoginDenied:
    {
        loginSuccessful = false;
        authorizationWindow->SetAuthorizationError("Login denied");
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
        client().FetchLobbies();
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
        client().FetchLobbies();
        break;
    }
    case ChessMessage::GameStarted:
    {
        std::cout << "Game Started";
        break;
    }
    case ChessMessage::GameStartDenied:
    {
        std::cout << "Game Start Denied";
        break;
    }
    default: break;
    }
}
