#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "ChessTypes.hpp"

namespace Engine { class Mesh; class Texture; }

class ChessPiece
{
public:
    ChessPiece(PlayerType player) : m_player(player) {}
    virtual ~ChessPiece() = default;

    virtual std::vector<std::vector<glm::vec2>> GetMoveOffsets() const = 0;
    virtual std::vector<std::vector<glm::vec2>> GetAdditionalAttackOffsets() const { return {}; }
    virtual bool IsMoveAttackSeparated() const { return true; }
    virtual bool CanCombine() const { return true; }

    virtual std::shared_ptr<Engine::Mesh> GetMesh() const = 0;
    virtual std::string GetTextureName() const = 0;

    Engine::Texture* GetTexture() const;

    const PlayerType& GetPlayer() const { return m_player; }

    static std::unique_ptr<ChessPiece> CreatePiece(PieceType type, PlayerType player);

protected:
    PlayerType m_player;

    const std::string whiteTexPrefix = "woodlig";
    const std::string blackTexPrefix = "wooddar";

};

class Pawn : public ChessPiece
{
public:
    using ChessPiece::ChessPiece;

    std::vector<std::vector<glm::vec2>> GetMoveOffsets() const override
    {
        std::vector<std::vector<glm::vec2>> moveOffsets;
        if (this->m_player == PlayerType::White)
        {
            if (m_hasMoved) 
                moveOffsets = { { {0, 1} } };
            else
                moveOffsets = { { {0, 1}, {0, 2} } };
        }
        else
        {
            if (m_hasMoved) 
                moveOffsets = { { {0,-1} } };
            else
                moveOffsets = { { {0,-1}, {0,-2} } };
        }

        return moveOffsets;
    }

    std::vector<std::vector<glm::vec2>> GetAdditionalAttackOffsets() const override
    {
        std::vector<std::vector<glm::vec2>> attackOffsets;
        if (this->m_player == PlayerType::White)
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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "0"; }

    void SetHasMoved() { m_hasMoved = true; }

private:
    bool m_hasMoved = false;

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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "2"; }
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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "3"; }
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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "1"; }
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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "4"; }
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

    std::shared_ptr<Engine::Mesh> GetMesh() const override;

    std::string GetTextureName() const override { return "5"; }
};
