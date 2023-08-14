#pragma once

enum class PieceType
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class MoveType
{
    Move,
    Capture
};

enum class PlayerType
{
    White,
    Black,
    Spectator
};