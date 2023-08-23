#include "GUIErrorText.hpp"

void GUIErrorText::PreRenderSetup()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
}

void GUIErrorText::AfterRenderSetup()
{
    ImGui::PopStyleColor();
}
