#pragma once

#include <vector>
#include "Engine.hpp"

enum BlockMesh_Enum { CUBE = 0 };

class Block : public Engine::GameObject
{
public:
    Block(uint32_t id, const char* textureFileName, glm::vec3 position);
    Block(uint32_t id, Engine::Texture* texture, glm::vec3 position);

    virtual ~Block() = default;

    void UpdateMeshSides(int blockFlags);

    uint32_t GetID();
private:
    uint32_t id;

    void CreateMesh();
};