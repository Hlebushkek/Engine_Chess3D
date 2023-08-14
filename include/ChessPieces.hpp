#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "ChessTypes.hpp"
#include "Engine.hpp"

class ChessPiece
{
public:
    ChessPiece(PlayerType player) : _player(player) {}
    virtual ~ChessPiece() = default;

    virtual std::vector<std::vector<glm::vec2>> GetMoveOffsets() const = 0;
    virtual std::vector<std::vector<glm::vec2>> GetAdditionalAttackOffsets() const { return {};}
    virtual bool IsMoveAttackSeparated() const { return true; }
    virtual bool CanCombine() const { return true; }

    virtual Engine::Mesh *GetMesh() const = 0;
    virtual Engine::Texture *GetTexture() const = 0;

    static ChessPiece *CreatePiece(PieceType type, PlayerType player);

    const PlayerType& GetPlayer() const { return _player; }

protected:
    PlayerType _player;

};

class Pawn : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        std::vector<std::vector<glm::vec2>> moveOffsets;
        if (this->_player == PlayerType::White)
            moveOffsets = { { {0, 1}, {0, 2} } };
        else
            moveOffsets = { { {0,-1}, {0,-2} } };

        return moveOffsets;
    }

    std::vector<std::vector<glm::vec2>> GetAdditionalAttackOffsets() const override
    {
        std::vector<std::vector<glm::vec2>> attackOffsets;
        if (this->_player == PlayerType::White)
            attackOffsets = { { {1, 1} }, { {-1, 1} } };
        else
            attackOffsets = { { {1,-1} }, { {-1,-1} } };

        return attackOffsets;
    }

    bool IsMoveAttackSeparated() const override
    {
        return false;
    }

    bool CanCombine() const override
    {
        return false;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/Pawn.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig0.bmp", GL_TEXTURE_2D);
    }
};

class Knight : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        static const std::vector<std::vector<glm::vec2>> moveOffsets = {
            {{1, 2}}, {{2, 1}}, {{2,-1}}, {{1,-2}}, {{-1, -2}}, {{-2, -1}}, {{-2, 1}}, {{-1, 2}}
        };
        return moveOffsets;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/Knight.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig2.bmp", GL_TEXTURE_2D);
    }
};

class Bishop : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        static const std::vector<std::vector<glm::vec2>> moveOffsets = []() {
            std::vector<std::vector<glm::vec2>> offsets(4);

            for (int i = 1; i < 8; i++)
            {
                offsets[0].emplace_back( i, i);
                offsets[1].emplace_back( i,-i);
                offsets[2].emplace_back(-i, i);
                offsets[3].emplace_back(-i,-i);
            }

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/Bishop.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig3.bmp", GL_TEXTURE_2D);
    }
};

class Rook : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        static const std::vector<std::vector<glm::vec2>> moveOffsets = []() {
            std::vector<std::vector<glm::vec2>> offsets(4);

            for (int i = 1; i < 8; i++)
            {
                offsets[0].emplace_back( 0, i);
                offsets[1].emplace_back( i, 0);
                offsets[2].emplace_back( 0,-i);
                offsets[3].emplace_back(-i, 0);
            }

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/Rook.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig1.bmp", GL_TEXTURE_2D);
    }
};

class Queen : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        static const std::vector<std::vector<glm::vec2>> moveOffsets = []() {
            std::vector<std::vector<glm::vec2>> offsets(12);
            for (int i = 1; i < 8; i++)
            {
                offsets[0].emplace_back( i, i);
                offsets[1].emplace_back( i,-i);
                offsets[2].emplace_back(-i, i);
                offsets[3].emplace_back(-i,-i);
                offsets[4].emplace_back( 0, i);
                offsets[5].emplace_back( i, 0);
                offsets[6].emplace_back( 0,-i);
                offsets[7].emplace_back(-i, 0);
            }

            offsets.emplace_back(std::vector<glm::vec2>{{ 0, 1}});
            offsets.emplace_back(std::vector<glm::vec2>{{ 1, 0}});
            offsets.emplace_back(std::vector<glm::vec2>{{ 0,-1}});
            offsets.emplace_back(std::vector<glm::vec2>{{-1, 0}});

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/Queen.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig4.bmp", GL_TEXTURE_2D);
    }
};

class King : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        static const std::vector<std::vector<glm::vec2>> moveOffsets = {
            {{0, 1}}, {{1, 1}}, {{1, 0}}, {{1, -1}}, {{0, -1}}, {{-1, -1}}, {{-1, 0}}, {{-1, 1}}
        };
        return moveOffsets;
    }

    Engine::Mesh *GetMesh() const override
    {
        static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("../resources/King.obj");
        return new Engine::Mesh(vertices.data(), vertices.size(), nullptr, 0);
    }

    Engine::Texture *GetTexture() const override
    {
        return Engine::Texture::LoadTexture("woodlig5.bmp", GL_TEXTURE_2D);
    }
};