#include <Engine.hpp>
#include <include/EntryPoint.hpp>
#include "Chess3D.hpp"
#include "InterfaceLayer.hpp"
#include "ChessBoardLayer.hpp"
#include "Block.hpp"
#include "TestGameObject.hpp"

Chess3D::Chess3D(const char* title, const int width, const int height)
    : Engine::Application(title, width, height)
{
    ChessBoardLayer* boardLayer = new ChessBoardLayer();
    PushLayer(boardLayer);
    
    Block* block = new Block(0, new Engine::Texture("Grass.png", GL_TEXTURE_2D), glm::vec3(1, 0, 0));
    boardLayer->AddRenderableObject(block);
    
    TestGameObject* testObject = new TestGameObject(glm::vec3(0), new Engine::Texture("Grass.png", GL_TEXTURE_2D));
    boardLayer->AddRenderableObject(testObject);
    
    InterfaceLayer* interfaceLayer = new InterfaceLayer();
    PushLayer(interfaceLayer);
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}