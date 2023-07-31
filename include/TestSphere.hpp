#pragma once

#include <vector>
#include "Engine.hpp"

class TestSphere : public Engine::GameObject
{
public:
    TestSphere(glm::vec3 position);

    void Render(Engine::Shader* shader) override;

private:
    Engine::Texture* texture;

};