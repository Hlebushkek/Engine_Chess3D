#include <Engine.hpp>
#include "ChessPieces.hpp"

std::unique_ptr<ChessPiece> ChessPiece::CreatePiece(PieceType type, PlayerType player)
{
    switch (type)
    {
        case PieceType::Pawn: return std::make_unique<Pawn>(player);
        case PieceType::Knight: return std::make_unique<Knight>(player);
        case PieceType::Bishop: return std::make_unique<Bishop>(player);
        case PieceType::Rook: return std::make_unique<Rook>(player);
        case PieceType::Queen: return std::make_unique<Queen>(player);
        case PieceType::King: return std::make_unique<King>(player);
        default: return std::make_unique<Pawn>(player);
    }
}

Engine::Texture* ChessPiece::GetTexture() const
{
    return Engine::Texture::LoadTexture(
        std::string(m_player == PlayerType::White ? whiteTexPrefix : blackTexPrefix).append(GetTextureName()).append(".bmp"));
}

std::shared_ptr<Engine::Mesh> Pawn::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("Pawn.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}

std::shared_ptr<Engine::Mesh> Knight::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("Knight.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}

std::shared_ptr<Engine::Mesh> Bishop::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("Bishop.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}

std::shared_ptr<Engine::Mesh> Rook::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("Rook.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}

std::shared_ptr<Engine::Mesh> Queen::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("Queen.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}

std::shared_ptr<Engine::Mesh> King::GetMesh() const
{
    static std::vector<Engine::Vertex> vertices = Engine::OBJClass().loadOBJ("King.obj");
    return std::make_shared<Engine::Mesh>(vertices.data(), vertices.size(), nullptr, 0);
}