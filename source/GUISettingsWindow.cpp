#include "GUISettingsWindow.hpp"

void GUISettingsWindow::InnerRender()
{
    ImGui::Dummy(ImVec2(0.0f, 1.0f));
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
    ImGui::Text("%s", SDL_GetPlatform());
    ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
    ImGui::Text("CPU cache size: %d", SDL_GetCPUCacheLineSize());
    ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);\

    GUIWindow::InnerRender();
}