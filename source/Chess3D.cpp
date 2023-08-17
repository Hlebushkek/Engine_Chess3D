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

    auto boardLayer = std::make_shared<ChessBoardLayer>();
    PushLayer(boardLayer);

    ChessBoard *board = new ChessBoard();
    boardLayer->AddRenderableObject(board);
    
    TestGameObject *testObject = new TestGameObject(glm::vec3(0), Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D));
    boardLayer->AddRenderableObject(testObject);

    TestSphere *testSphere = new TestSphere(glm::vec3(0.4f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testSphere);

    Block *testBlock = new Block(0, "BlockB.png", glm::vec3(0.25f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testBlock);

    // AddLight(new Engine::PointLight(glm::vec3(1.0f, 0.2f, -0.25f)));
    // AddLight(new Engine::PointLight(glm::vec3(0.5f, 0.2f, -0.25f)));
    AddLight(new Engine::SpotLight(glm::vec3(0.71875f, 0.4f, -0.21875f)));
    // AddLight(new Engine::DirectionalLight(glm::vec3(0.f, -0.5f, -0.5f)));

    PushLayer(std::make_shared<InterfaceLayer>());
}

void Chess3D::Update()
{
    Engine::Application::Update();
    UpdateNetwork();
}

void Chess3D::UpdateNetwork()
{
    if (client.isConnected())
    {
        if (!client.incoming().empty())
        {
            auto msg = client.incoming().pop_front().msg;
            netMsgDispatcher.DispatchMessage(msg);

            switch (msg.header.id)
            {
            case ChessMessage::ServerPing:
            {
                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point timeThen;
                msg >> timeThen;
                std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				break;
            }
            default: break;
            }
        }
    }
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}