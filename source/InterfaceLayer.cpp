
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#elif __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

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
{}

void InterfaceLayer::OnImGuiRender()
{
    ImGui::SetCurrentContext(Engine::ImGuiLayer::GetImguiContext());

    struct User {
    std::string username;
    std::string password;
    };

    std::vector<User> users =
    {
    {"kali", "kali"},
    {"Ostrich", "Warlock"},
    {"Hlebo", "Bulka"}
    };

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
    ImGui::SetNextWindowSize(ImVec2(250,300));
    windowFlags = 0;
    if (isLogInOpen)
    {   
        ImGui::Begin("Welcome to the club, buddy!", &isLogInOpen, windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
        ImGui::Text("User: ");
        if(ImGui::InputText(("max " + std::to_string(sizeof(buffer_user) / sizeof(char) - 1) + "##user").c_str(), buffer_user, sizeof(buffer_user)))
        {

        }
        ImGui::Text("Password: ");
        ImGui::Checkbox("Show Password", &showPass);
        ImGui::InputText(("max " + std::to_string(sizeof(buffer_pass) / sizeof(char) - 1) + "##pass").c_str(), buffer_pass, sizeof(buffer_pass), !showPass ? ImGuiInputTextFlags_Password : 0);
        for (const User& user : users) 
        {
            if (user.username == buffer_user && user.password == buffer_pass) {
                loginSuccessful = true;
                break;}
            else{
                loginSuccessful = false;}
        }
        if(ImGui::Button("Register"))
        {
            std::cout << "Login: " << buffer_user << std::endl;
            std::cout << "Password: " << buffer_pass << std::endl;

            (static_cast<Chess3D*>(Chess3D::Get()))->client.Register(buffer_user, buffer_pass);

            if (loginSuccessful)
            {
                isLobbiesOpen = true;
                isLogInOpen = false;
            }
        }
        if(ImGui::Button("Login"))
        {   
            std::cout << "Login: " << buffer_user << std::endl;
            std::cout << "Password: " << buffer_pass << std::endl;

            (static_cast<Chess3D*>(Chess3D::Get()))->client.LoginIn(buffer_user, buffer_pass);

            if (loginSuccessful)
            {
                isLobbiesOpen = true;
                isLogInOpen = false;
            }
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
        ImGui::SetNextWindowSize(ImVec2(500,500));
        ImGui::Begin("Lobby selection", &isLobbiesOpen, ImGuiWindowFlags_MenuBar);
        style.ItemSpacing.x = 70.0f; 
        if (ImGui::BeginMenuBar())
        {   
            if (ImGui::Button("Name")) {}
            if (ImGui::Button("GameType")) {}
            if (ImGui::Button("Players")) {}

            ImGui::EndMenuBar();
        }
        ImGui::Text("Hikaru Nakamura");
        ImGui::SameLine();
        ImGui::Text("Bullet 3:2");
        ImGui::SameLine();
        ImGui::Text("1/2");
        ImGui::SameLine();
        ImGui::Button("Join");
        ImGui::End();
    }

    style = ImGuiStyle();


    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);         // Dark gray menu bar background
    style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);    // Hovered color for menu items
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);     // Active (pressed) color for menu items  
    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("System"))
        {
            if (ImGui::MenuItem("Log In", nullptr, &isLogInOpen, true)){}
            if (ImGui::MenuItem("Settings", nullptr, &isSettingsOpen, true)){}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    style = ImGuiStyle();

}
