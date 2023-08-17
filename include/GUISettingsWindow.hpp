#pragma once

#include <Engine.hpp>

class GUISettingsWindow : public Engine::GUIWindow
{
public:
    GUISettingsWindow() : GUIWindow("Settings") {}

    void InnerRender() override;

};