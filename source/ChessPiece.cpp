#include "ChessPiece.hpp"

ChessPiece::ChessPiece(Piece *pieceModel, glm::vec3 position)
    : GameObject(position, glm::vec3(0.f), glm::vec3(0.0625f))
{
    this->meshes.emplace_back(pieceModel->getMesh());
    this->texture = Engine::Texture::LoadTexture("Brick.png", GL_TEXTURE_2D);
}

ChessPiece::~ChessPiece()
{
    delete texture;
}

void ChessPiece::Render(Engine::Shader *shader)
{
    texture->bind(0);
    GameObject::Render(shader);
    texture->unbind(0);
}
