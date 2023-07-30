#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include "InterfaceLayer.hpp"
#include "Chess3D.hpp"
#include "Engine.hpp"


InterfaceLayer::InterfaceLayer() : Engine::Layer("InterfaceLayer"), myButton("Button", ImVec2(10, 50), ImVec2(100, 100), "../resources/button.png", []() {
    std::cout << "Good day sir!\n";
    })
{
}

void InterfaceLayer::OnImGuiRender()
{
    ImGui::SetCurrentContext(Engine::ImGuiLayer::GetImguiContext());
    ImGui::Begin("Settings");

    static int counter = 0;
    
    int sdl_width = 300, sdl_height = 300, controls_width = 0;
    controls_width = sdl_width;
    if ((controls_width /= 3) < 300) { controls_width = 300; }

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
        ImGuiCond_Always
        );

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
    ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_Always);
    ImGuiWindowFlags windowFlags = 0;
    if (ImGui::Begin("Buttons", nullptr, windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) // ImGuiWindowFlags_NoCollapse, ImGuiWindowFlags_NoMove and ImGuiWindowFlags_NoResize are not mandatory, it works without them. 
    {
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        myButton.Render();
    }
    ImGui::End();
}