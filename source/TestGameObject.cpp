#include "TestGameObject.hpp"

TestGameObject::TestGameObject(glm::vec3 position, Engine::Texture* texture) : GameObject(position, glm::vec3(0.f), glm::vec3(0.1f))
{
    this->texture = texture;

    auto vertices = Engine::OBJClass().loadOBJ("../resources/guurl.obj");

    std::vector<glm::vec3> indices;
    Engine::Mesh *mesh = new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    this->meshes.emplace_back(mesh);
}

void TestGameObject::Render(Engine::Shader* shader)
{
    // texture->bind(0);
    GameObject::Render(shader);
    // texture->unbind(0);
}