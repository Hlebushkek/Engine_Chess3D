
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

ChessClient *client() { return (static_cast<Chess3D*>(Chess3D::Get()))->GetClient(); }

InterfaceLayer::InterfaceLayer()
    : Engine::Layer("InterfaceLayer"), buttonGit("##gitButton", ImVec2(90, 90), "github_icon.png")
{
    buttonGit.SetCustomPosition(ImVec2(9, 10));
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
    lobbyWindow = std::make_shared<GUILobbyWindow>();
    this->windows.push_back(settingsWindow);
    this->windows.push_back(authorizationWindow);
    this->windows.push_back(lobbiesListWindow);
    this->windows.push_back(lobbyWindow);
}

void InterfaceLayer::OnAttach()
{
    authorizationWindow->delegate = weak_from_this();
    lobbiesListWindow->delegate = weak_from_this();
    lobbyWindow->delegate = weak_from_this();
    
    (static_cast<Chess3D*>(Chess3D::Get()))->GetNetMessageDispatcher().AddListener(weak_from_this());
    (static_cast<Chess3D*>(Chess3D::Get()))->GetNetMessageDispatcher().AddListener(lobbiesListWindow);

    auto ctx = Engine::Renderer::GetRenderingContext();
    ImGui::SetCurrentContext(ctx->imguiContext);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Button] = ImVec4(0.7f, 0.7f, 0.7f, 0.1f);  
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);  
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 0.5f); 
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.7f); 
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
}

void InterfaceLayer::OnImGuiRender()
{
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
 
    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("System"))
        {
            ImGui::MenuItem("Authorization", nullptr, &authorizationWindow->visible, true);
            ImGui::MenuItem("Lobby", nullptr, &lobbiesListWindow->visible, loginSuccessful && !gameInProgress);
            ImGui::MenuItem("Settings", nullptr, &settingsWindow->visible, true);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void InterfaceLayer::OnLogin(const std::string& user, const std::string& password)
{
    client()->LoginIn(user, password);
}

void InterfaceLayer::OnRegister(const std::string &email, const std::string &user, const std::string &password)
{
    client()->Register(email, user, password);
}

void InterfaceLayer::OnLogout()
{
    lobbyWindow->visible = false;
    lobbiesListWindow->visible = false;
    currentLobby = {};
    loginSuccessful = false;
    user = {};
}

void InterfaceLayer::OnLobbiesFetch()
{   
    client()->FetchLobbies();
}

void InterfaceLayer::OnLobbyCreate(std::string &name, std::string &password)
{
    client()->CreateLobby(name, password, user.id);
}

void InterfaceLayer::OnLobbyJoin(Lobby& lobby)
{
    client()->JoinLobby(user.id, lobby);
}

void InterfaceLayer::OnLobbySpectate(int lobby_id)
{
    client()->SpectateLobby(lobby_id, currentLobby.id);
}

void InterfaceLayer::OnGameStart()
{
    client()->StartGame(currentLobby.id);
}

void InterfaceLayer::OnLeave()
{
    client()->LeaveLobby(user.id, currentLobby);
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
        lobbiesListWindow->visible = true;

        authorizationWindow->SetAuthorizationSuccess();
        authorizationWindow->visible = false;

        client()->FetchLobbies();
        break;
    }
    case ChessMessage::LoginDenied:
    {
        auto msgCopy = msg;
        std::string response;
        msgCopy >> response;
        loginSuccessful = false;
        authorizationWindow->SetAuthorizationError(response);
        break;
    }
    case ChessMessage::LobbyJoined:
    {
        auto msgCopy = msg;
        msgCopy >> currentLobby;
        lobbyWindow->UpdateLobby(currentLobby);
        lobbyWindow->visible = true;
        std::cout << "Lobby joined. " << currentLobby.name << ", " << currentLobby.password << std::endl;
        client()->FetchLobbies();
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
        lobbyWindow->visible = false;
        currentLobby = {};
        client()->FetchLobbies();
        break;
    }
    case ChessMessage::LobbyUpdated:
    {
        auto msgCopy = msg;
        msgCopy >> currentLobby;
        lobbyWindow->UpdateLobby(currentLobby);
        break;
    }
    case ChessMessage::GameStarted:
    {
        gameInProgress = true;
        lobbyWindow->visible = false;
        lobbiesListWindow->visible = false;
        break;
    }
    case ChessMessage::GameStartDenied:
    {
        std::cout << "Game Start Denied\n";
        break;
    }
    default: break;
    }
}
