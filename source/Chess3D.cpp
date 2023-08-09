#include <Engine.hpp>
#include <include/EntryPoint.hpp>
#include "Chess3D.hpp"
#include "InterfaceLayer.hpp"
#include "ChessBoardLayer.hpp"
#include "Block.hpp"
#include "TestSphere.hpp"
#include "TestGameObject.hpp"
#include "ChessBoard.hpp"

Chess3D::Chess3D(const char* title, const int width, const int height)
    : Engine::Application(title, width, height)
{
    client.connect("127.0.0.1", 60000);

    ChessBoardLayer* boardLayer = new ChessBoardLayer();
    PushLayer(boardLayer);

    ChessBoard *board = new ChessBoard();
    boardLayer->AddRenderableObject(board);
    
    TestGameObject *testObject = new TestGameObject(glm::vec3(0), Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D));
    boardLayer->AddRenderableObject(testObject);

    TestSphere *testSphere = new TestSphere(glm::vec3(0.4f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testSphere);

    Block *testBlock = new Block(0, "BlockB.png", glm::vec3(0.25f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testBlock);
    
    InterfaceLayer* interfaceLayer = new InterfaceLayer();
    PushLayer(interfaceLayer);
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}