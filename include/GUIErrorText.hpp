#pragma once

#include <Engine.hpp>

class GUIErrorText : public Engine::GUIText
{
public:
    GUIErrorText(const std::string& text = "") : GUIText(text) {}

    void PreRenderSetup() override;
    void AfterRenderSetup() override;

};