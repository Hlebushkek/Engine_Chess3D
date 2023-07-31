#include "TestGameObject.hpp"
#include "Engine.hpp"

TestGameObject::TestGameObject(glm::vec3 position, Engine::Texture* texture) : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    this->texture = texture;
    this->collider = new Engine::SphereCollider(0.5f, glm::vec3(0.f, 0.1f, 0.f));

    auto vertices = Engine::OBJClass().loadOBJ("../resources/guurl.obj");
    Engine::Mesh *mesh = new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    this->meshes.emplace_back(mesh);
}

void TestGameObject::Render(Engine::Shader* shader)
{
    // texture->bind(0);
    GameObject::Render(shader);
    // texture->unbind(0);
}