#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include "../include/InterfaceLayer.hpp"
#include "../include/Chess3D.hpp"

InterfaceLayer::InterfaceLayer() : Layer("InterfaceLayer") {}

void InterfaceLayer::OnAttach()
{
    
}

void InterfaceLayer::OnDetach()
{

}

void InterfaceLayer::Update()
{
    
}

void InterfaceLayer::OnImGuiRender() 
{
    ImGui::SetCurrentContext(Chess3D::Get()->imguiContext);
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
}