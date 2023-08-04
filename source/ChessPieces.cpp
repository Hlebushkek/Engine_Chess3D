#include "ChessPieces.hpp"

ChessPiece *ChessPiece::CreatePiece(PieceType type, PlayerType player)
{
    switch (type)
    {
        case PAWN: return new Pawn(player);
        case KNIGHT: return new Knight(player);
        case BISHOP: return new Bishop(player);
        case ROOK: return new Rook(player);
        case QUEEN: return new Queen(player);
        case KING: return new King(player);
        default: return new Pawn(player);
    }
}