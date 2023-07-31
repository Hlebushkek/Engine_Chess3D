#include "TestSphere.hpp"

TestSphere::TestSphere(glm::vec3 position) : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    Engine::Primitive sphere = Engine::Sphere();
    this->meshes.emplace_back(new Engine::Mesh(sphere));
}

void TestSphere::Render(Engine::Shader* shader)
{
    // texture->bind(0);
    GameObject::Render(shader);
    // texture->unbind(0);
}