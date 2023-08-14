#include "ChessPieces.hpp"

ChessPiece *ChessPiece::CreatePiece(PieceType type, PlayerType player)
{
    switch (type)
    {
        case PieceType::Pawn: return new Pawn(player);
        case PieceType::Knight: return new Knight(player);
        case PieceType::Bishop: return new Bishop(player);
        case PieceType::Rook: return new Rook(player);
        case PieceType::Queen: return new Queen(player);
        case PieceType::King: return new King(player);
        default: return new Pawn(player);
    }
}