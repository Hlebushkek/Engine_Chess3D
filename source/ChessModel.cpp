#include "ChessModel.hpp"

ChessModel::ChessModel()
{
    for (int i = 0; i < 8; i++)
    {
        boardState[1][i] = ChessPiece::CreatePiece(PieceType::Pawn, PlayerType::White);
        boardState[6][i] = ChessPiece::CreatePiece(PieceType::Pawn, PlayerType::Black);
    }

    boardState[0][0] = ChessPiece::CreatePiece(PieceType::Rook, PlayerType::White);
    boardState[0][7] = ChessPiece::CreatePiece(PieceType::Rook, PlayerType::White);
    boardState[7][0] = ChessPiece::CreatePiece(PieceType::Rook, PlayerType::Black);
    boardState[7][7] = ChessPiece::CreatePiece(PieceType::Rook, PlayerType::Black);

    boardState[0][1] = ChessPiece::CreatePiece(PieceType::Knight, PlayerType::White);
    boardState[0][6] = ChessPiece::CreatePiece(PieceType::Knight, PlayerType::White);
    boardState[7][1] = ChessPiece::CreatePiece(PieceType::Knight, PlayerType::Black);
    boardState[7][6] = ChessPiece::CreatePiece(PieceType::Knight, PlayerType::Black);
    
    boardState[0][2] = ChessPiece::CreatePiece(PieceType::Bishop, PlayerType::White);
    boardState[0][5] = ChessPiece::CreatePiece(PieceType::Bishop, PlayerType::White);
    boardState[7][2] = ChessPiece::CreatePiece(PieceType::Bishop, PlayerType::Black);
    boardState[7][5] = ChessPiece::CreatePiece(PieceType::Bishop, PlayerType::Black);

    boardState[0][3] = ChessPiece::CreatePiece(PieceType::Queen, PlayerType::White);
    boardState[7][3] = ChessPiece::CreatePiece(PieceType::Queen, PlayerType::Black);
    
    boardState[0][4] = ChessPiece::CreatePiece(PieceType::King, PlayerType::White);
    boardState[7][4] = ChessPiece::CreatePiece(PieceType::King, PlayerType::Black);

    for (int y = 2; y < 6; y++)
        for (int x = 0; x < 8; x++)
            boardState[y][x] = nullptr;
}

std::vector<std::pair<glm::ivec2, MoveType>> ChessModel::GetSelectionFor(ChessPiece *piece)
{
    std::vector<std::pair<glm::ivec2, MoveType>> result;

    PlayerType player = piece->GetPlayer();
    glm::vec2 position = GetPositionFor(piece);

    auto availableAdditionalAttackOffsets = piece->GetAdditionalAttackOffsets();
    for (const auto& offsetsLine : availableAdditionalAttackOffsets)
    {
        for (const auto& offset : offsetsLine)
        {
            glm::ivec2 testPosition = glm::ivec2(position + offset);
            if (!PositionExists(testPosition))
                break;

            if (ChessPiece *otherPiece = GetPieceAt(testPosition))
                if (otherPiece->GetPlayer() != player)
                    result.emplace_back(testPosition, MoveType::Capture);
        }
    }

    bool isMoveAttackSeparated = availableAdditionalAttackOffsets.size() > 0;
    auto availableOffsets = piece->GetMoveOffsets();
    for (const auto& offsetsLine : availableOffsets)
    {
        for (const auto& offset : offsetsLine)
        {
            glm::ivec2 testPosition = glm::ivec2(position + offset);
            if (!PositionExists(testPosition))
                break;

            if (ChessPiece *otherPiece = GetPieceAt(testPosition))
            {
                if (otherPiece->GetPlayer() != player && !isMoveAttackSeparated)
                    result.emplace_back(testPosition, MoveType::Capture);

                break;
            }
            else { result.emplace_back(testPosition, MoveType::Move); }
        }
    }

    return result;
}

void ChessModel::MovePiece(glm::ivec2 from, glm::ivec2 to)
{
    MovePiece(from.x, from.y, to.x, to.y);
}

void ChessModel::MovePiece(int fromX, int fromY, int toX, int toY)
{
    if (boardState[toY][toX])
        delete boardState[toY][toX];

    boardState[toY][toX] = boardState[fromY][fromX];
    boardState[fromY][fromX] = nullptr;

    if (auto pawn = static_cast<Pawn*>(boardState[toY][toX]))
        pawn->SetHasMoved();
}

ChessPiece *ChessModel::GetPieceAt(glm::ivec2 position)
{
    return GetPieceAt(position.x, position.y);
}
ChessPiece *ChessModel::GetPieceAt(int x, int y)
{
    if (!PositionExists(x, y))
        return nullptr;

    return boardState[y][x];
}

bool ChessModel::PositionExists(glm::ivec2 position)
{
    return PositionExists(position.x, position.y);
}

bool ChessModel::PositionExists(int x, int y)
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

glm::vec2 ChessModel::GetPositionFor(ChessPiece *piece)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (boardState[y][x] == piece)
                return glm::vec2(x, y);

    return glm::vec2(INT_MAX);
}