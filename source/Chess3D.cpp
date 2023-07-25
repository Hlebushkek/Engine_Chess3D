#include <Engine.hpp>
#include <include/EntryPoint.hpp>
#include "Chess3D.hpp"
#include "InterfaceLayer.hpp"
#include "ChessBoardLayer.hpp"
#include "Block.hpp"
#include "TestGameObject.hpp"
#include "ChessBoard.hpp"

Chess3D::Chess3D(const char* title, const int width, const int height)
    : Engine::Application(title, width, height)
{
    ChessBoardLayer* boardLayer = new ChessBoardLayer();
    PushLayer(boardLayer);

    ChessBoard *board = new ChessBoard();
    boardLayer->AddRenderableObject(board);
    
    TestGameObject* testObject = new TestGameObject(glm::vec3(0), new Engine::Texture("Grass.png", GL_TEXTURE_2D));
    boardLayer->AddRenderableObject(testObject);
    
    InterfaceLayer* interfaceLayer = new InterfaceLayer();
    PushLayer(interfaceLayer);
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}