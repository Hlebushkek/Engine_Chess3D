#include "Pieces.hpp"

Piece *Piece::CreatePiece(PieceType type)
{
    switch (type)
    {
        case PAWN: return new Pawn();
        case KNIGHT: return new Knight();
        case BISHOP: return new Bishop();
        case ROOK: return new Rook();
        case QUEEN: return new Queen();
        case KING: return new King();
        default: return new Pawn();
    }
}