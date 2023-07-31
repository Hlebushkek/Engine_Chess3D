#include "ChessBoard.hpp"
#include "ChessPiece.hpp"
#include "Pieces.hpp"
#include "Block.hpp"

ChessBoard::ChessBoard()
{
    textureWhite = Engine::Texture::LoadTexture("BlockB.png", GL_TEXTURE_2D);
    textureBlack = Engine::Texture::LoadTexture("Grass.png", GL_TEXTURE_2D);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            Engine::Texture *pieceTexture = ((i % 8 + j) % 2) == 0 ? textureWhite : textureBlack;
            Engine::GameObject *piece = new Block(0, pieceTexture, glm::vec3(0.5 + i * 0.0625, 0, j * -0.0625));
            boardBlocks.emplace_back(piece);
        }

    for (int i = 0; i < 8; i++)
    {
        Engine::GameObject *piece = new ChessPiece(new Pawn(), glm::vec3(0.5 + i * 0.0625, 0.0625, 0));
        pieces.emplace_back(piece);
    }
}

ChessBoard::~ChessBoard()
{
    delete textureWhite;
    delete textureBlack;
}

void ChessBoard::Render(Engine::Shader *shader)
{
    GameObject::Render(shader);

    for (auto obj : boardBlocks)
        obj->Render(shader);

    for (auto obj : pieces)
        obj->Render(shader);
}

std::optional<Engine::Intersection> ChessBoard::CollidesWith(const Ray &ray)
{
    for (auto obj : boardBlocks)
    {
        auto result = obj->CollidesWith(ray);
        if (result.has_value())
            return result;
    }

    for (auto obj : pieces)
    {
        auto result = obj->CollidesWith(ray);
        if (result.has_value())
            return result;
    }

    return std::nullopt;
}
