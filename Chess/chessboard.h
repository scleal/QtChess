#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <array>
#include "chesspiece.h"
#include "chessmove.h"
class ChessPiece;
class ChessMove;

class ChessBoard{
public:
    ChessBoard();
    ChessBoard(const ChessBoard&);
    ChessPiece* GetPiece(int, int) const;
    ChessBoard* DoMove(ChessPiece*, const ChessMove&);
    void Draw();
    void InitializeBoard();
    bool UnderAttack(ChessColor, int, int) const;
    bool InCheck(ChessColor) const;
    bool NoValidMoves(ChessColor) const;
    void MovePiece(ChessPiece*, int, int);
private:
    std::array<std::array<ChessPiece*, 8>, 8> chessBoard;
    ChessMove* GetKingPosition(ChessColor) const;
    void ClearCell(int, int);
};

#endif // CHESSBOARD_H
