#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include "chessboard.h"
class ChessBoard;

class ChessMove{
public:
    ChessMove(const int, const int, const int, const int);
    bool PutsInCheck(const ChessBoard&) const;
    bool IsValidMove(const ChessBoard&) const;
    bool IsValidEnPassantMove(const ChessBoard&, int, int) const;
    bool IsValidCastleMove(const ChessBoard&) const;
    const int startX, startY, endX, endY;
private:
    static bool GetCondition(int, int, int);
    bool GetConditionBishop(int, int) const;
    bool IsValidMovePawn(const ChessBoard&) const;
    bool IsValidMoveRook(const ChessBoard&) const;
    bool IsValidMoveBishop(const ChessBoard&) const;
    bool IsValidMoveKing() const;
    bool IsValidMoveKnight() const;
};

#endif // CHESSMOVE_H
