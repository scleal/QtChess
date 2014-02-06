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
    ~ChessBoard();
    ChessPiece* GetPiece(int, int) const;
    ChessBoard* DoMove(const ChessMove&);
    void InitializeBoard();
    bool UnderAttack(ChessColor, int, int) const;
    bool InCheck(ChessColor) const;
    bool NoValidMoves(ChessColor);
private:
    void MovePiece(ChessPiece*, int, int);
    void ClearCell(int, int);
    std::array<std::array<ChessPiece*, 8>, 8> chessBoard;
    ChessMove* GetKingPosition(ChessColor) const;
};

#endif // CHESSBOARD_H
