#include <Engine.hpp>
#include <include/EntryPoint.hpp>
#include "../include/Chess3D.hpp"
#include "../include/InterfaceLayer.hpp"

Chess3D::Chess3D(const char* title, const int width, const int height)
    : Engine::Application(title, width, height)
{
    InterfaceLayer* interfaceLayer = new InterfaceLayer();
    PushLayer(interfaceLayer);
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}