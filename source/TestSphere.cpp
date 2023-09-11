#include "TestSphere.hpp"

TestSphere::TestSphere(glm::vec3 position) : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    Engine::Primitive sphere = Engine::Sphere();
    this->meshes.emplace_back(std::make_shared<Engine::Mesh>(sphere));
}