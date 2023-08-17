#include "Block.hpp"

Block::Block(uint32_t id, const char* textureFileName, glm::vec3 position)
    : Block(id, Engine::Texture::LoadTexture(textureFileName, GL_TEXTURE_2D), position) {}

Block::Block(uint32_t id, Engine::Texture* texture, glm::vec3 position)
    : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    this->id = id;
    this->textures.push_back(texture);
    this->collider = new Engine::BoxCollider();

    CreateMesh();
}

uint32_t Block::GetID()
{
    return this->id;
}

void Block::CreateMesh()
{
    Engine::Primitive cube = Engine::Cube();
    this->meshes.emplace_back(new Engine::Mesh(cube));
}

void Block::UpdateMeshSides(int blockFlags)
{
    if (this->meshes[CUBE])
        delete this->meshes[CUBE];

    Engine::Primitive cube = Engine::Cube(blockFlags);
    this->meshes[CUBE] = new Engine::Mesh(cube);
}