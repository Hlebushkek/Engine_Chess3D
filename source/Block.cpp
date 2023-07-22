#include "Block.hpp"

Block::Block(uint32_t id, const char* textureFileName, glm::vec3 position) : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    this->id = id;
    this->texture = new Engine::Texture(textureFileName, GL_TEXTURE_2D);

    CreateMesh(position);

    this->collider = new Engine::BoxCollider(glm::vec3(0.0625f));
}

Block::Block(uint32_t id, Engine::Texture* texture, glm::vec3 position) : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    this->id = id;
    this->texture = texture;

    CreateMesh(position);

    this->collider = new Engine::BoxCollider(glm::vec3(0.0625f));
}

Block::~Block()
{
    // delete texture;
}

uint32_t Block::GetID()
{
    return this->id;
}

void Block::CreateMesh(glm::vec3 position)
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

void Block::Render(Engine::Shader* shader)
{
    texture->bind(0);
    GameObject::Render(shader);
    texture->unbind(0);
}