#pragma once

#include <vector>
#include "Engine.hpp"

class TestGameObject : public Engine::GameObject
{
public:
    TestGameObject(glm::vec3 position, Engine::Texture* texture);
    void Render(Engine::Shader* shader) override;

private:
    Engine::Texture* texture;

};