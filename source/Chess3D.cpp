#include <Engine.hpp>
#include <include/EntryPoint.hpp>
#include "Chess3D.hpp"
#include "InterfaceLayer.hpp"
#include "ChessBoardLayer.hpp"
#include "Block.hpp"
#include "TestSphere.hpp"
#include "TestGameObject.hpp"
#include "ChessBoard.hpp"
#include "ChessSessionOffline.hpp"
#include "ChessSessionOnline.hpp"

Chess3D::Chess3D(const char* title, const int width, const int height)
    : Engine::Application(title, width, height)
{
    client = std::make_shared<ChessClient>();
    client->connect("127.0.0.1", 60000);

    auto boardLayer = std::make_shared<ChessBoardLayer>();
    PushLayer(boardLayer);

    board = Engine::GameObject::Instantiate<ChessBoard>();
    boardLayer->AddRenderableObject(board);
    
    std::shared_ptr<TestGameObject> testObject = Engine::GameObject::Instantiate<TestGameObject>(glm::vec3(0.f), Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D));
    boardLayer->AddRenderableObject(testObject);

    std::shared_ptr<TestSphere> testSphere = Engine::GameObject::Instantiate<TestSphere>(glm::vec3(0.4f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testSphere);

    std::shared_ptr<Block> testBlock = Engine::GameObject::Instantiate<Block>(0, "BlockB.png", glm::vec3(0.25f, 0.f, 0.f));
    boardLayer->AddRenderableObject(testBlock);

    // AddLight(new Engine::PointLight(glm::vec3(1.0f, 0.2f, -0.25f)));
    // AddLight(new Engine::PointLight(glm::vec3(0.5f, 0.2f, -0.25f)));
    AddLight(new Engine::SpotLight(glm::vec3(0.71875f, 0.4f, -0.21875f)));
    // AddLight(new Engine::DirectionalLight(glm::vec3(0.f, -0.5f, q-0.5f)));

    PushLayer(std::make_shared<InterfaceLayer>());

    SetupSession(std::make_shared<ChessSessionOffline>(board));
}

void Chess3D::Update()
{
    Engine::Application::Update();
    UpdateNetwork();
}

void Chess3D::UpdateNetwork()
{
    if (client->isConnected())
    {
        if (!client->incoming().empty())
        {
            auto msg = client->incoming().pop_front().msg;
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
            case ChessMessage::LoginAccepted:
            {
                auto msgCopy = msg;
                msgCopy >> activeUser;
                break;
            }
            case ChessMessage::LobbyJoined:
            {
                auto msgCopy = msg;
                msgCopy >> activeLobby;
                break;
            }
            case ChessMessage::LobbyUpdated:
            {
                auto msgCopy = msg;
                msgCopy >> activeLobby;
                break;
            }
            case ChessMessage::GameStarted:
            {
                std::cout << "Game started\n";
                SetupSession(std::make_shared<ChessSessionOnline>(client, board, activeLobby, activeUser));
                break;
            }
            default: break;
            }
        }
    }
}

void Chess3D::SetupSession(std::shared_ptr<ChessSession> session)
{
    activeSession = session;
    activeSession->Start();
}

void Chess3D::ApplicationWillTerminate()
{
    if (activeLobby.id)
        client->LeaveLobby(activeUser.id, activeLobby);
    activeSession = nullptr;
}

Engine::Application* Engine::CreateApplication()
{
    return new Chess3D("Chess3D", 600, 400);
}