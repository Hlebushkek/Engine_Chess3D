#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <AppKit/AppKit.h>
#endif

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include "InterfaceLayer.hpp"
#include "Chess3D.hpp"
#include "Engine.hpp"

#ifdef _WIN32
void OpenURLWindows(const char* url) {
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
#endif

#ifdef __APPLE__
void OpenURLMac(const char* url) {
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}
#endif

bool isWindow1Open = true; 
bool isWindow2Open = true; 
char buffer_user[11] = "";
char buffer_pass[11] = "";
bool isChecked1 = false;
bool showPass = false;

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

    if (isWindow2Open)
    {
        ImGui::Begin("Settings", &isWindow2Open);
        
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

    ImGui::SetNextWindowPos(ImVec2(50,920));
    ImGuiWindowFlags windowFlags = 0;
    if (ImGui::Begin("Buttons", nullptr, windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |  ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysAutoResize )) // ImGuiWindowFlags_NoCollapse, ImGuiWindowFlags_NoMove and ImGuiWindowFlags_NoResize are not mandatory, it works without them. 
    {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        buttonGit.Render();
    }
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(300,360));
    windowFlags = 0;
    if (isWindow1Open)
    {
        ImGui::Begin("Welcome to the club, buddy!", &isWindow1Open, windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
        ImGui::Text("User: ");
        if(ImGui::InputText("max 10 ", buffer_user, sizeof(buffer_user)))
        {

        }
        ImGui::BeginGroup();
        ImGui::Text("Password: ");
        ImGui::Checkbox("Show Password", &showPass);
        
        if (!showPass) 
        {
            ImGui::InputText("max 10", buffer_pass, sizeof(buffer_pass), ImGuiInputTextFlags_Password);
        }
        else
        {
            ImGui::InputText("max 10", buffer_pass, sizeof(buffer_pass));
        }
        ImGui::EndGroup();
        if(ImGui::Button("Login"))
        {
            std::cout << "Login: " << buffer_user << std::endl;
            std::cout << "Password: " << buffer_pass << std::endl;
        }
        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("System"))
        {
            if (ImGui::MenuItem("Log In", nullptr, &isWindow1Open, true))
            {
                // isWindow1Open = true;  
            }
            if (ImGui::MenuItem("Settings", nullptr, &isWindow2Open, true))
            {
                // isWindow2Open = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}
