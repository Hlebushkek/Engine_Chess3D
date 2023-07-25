#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "ChessTypes.hpp"
#include "Engine.hpp"

class Piece
{
public:
    Piece() {}
    virtual ~Piece() = default;

    virtual std::vector<glm::vec2> getMoveOffsets() const = 0;
    virtual Engine::Mesh *getMesh() const = 0;

    static Piece *CreatePiece(PieceType type);
};

class Pawn : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = { {0, 1}, {0, 2} };
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};

class Knight : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = { {1, 2}, {2, 1}, {2,-1}, {1,-2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2} };
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};

class Bishop : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = []() {
            std::vector<glm::vec2> offsets;
            const int start = -7;
            const int end = 7;

            for (int i = start; i <= end; i++)
            {
                if (i == 0) continue;

                offsets.emplace_back(i, i);
                offsets.emplace_back(i,-i);
            }

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};

class Rook : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = []() {
            std::vector<glm::vec2> offsets;
            const int start = -7;
            const int end = 7;

            for (int i = start; i <= end; i++)
            {
                if (i == 0) continue;

                offsets.emplace_back(0, i);
                offsets.emplace_back(i, 0);
            }

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};

class Queen : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = []() {
            std::vector<glm::vec2> offsets;
            const int start = -7;
            const int end = 7;

            for (int i = start; i <= end; i++)
            {
                if (i == 0) continue;

                offsets.emplace_back(i, i);
                offsets.emplace_back(i,-i);
            }

            offsets.emplace_back(0, 1);
            offsets.emplace_back(1, 0);
            offsets.emplace_back(0,-1);
            offsets.emplace_back(-1,0);

            return offsets;
        }();
        
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};

class King : public Piece
{
public:
    std::vector<glm::vec2> getMoveOffsets() const override
    {
        static const std::vector<glm::vec2> moveOffsets = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };
        return moveOffsets;
    }

    Engine::Mesh *getMesh() const override
    {
        Engine::Primitive cube = Engine::Cube();
        return new Engine::Mesh(cube);
    }
};