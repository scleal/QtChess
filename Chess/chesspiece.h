#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <memory>
#include <QImage>
#include "chessrank.h"
#include "chesscolor.h"

class ChessPiece{
public:
    ChessPiece(ChessRank, ChessColor, const char*);
    ChessPiece(const ChessPiece&);
    ~ChessPiece();
    inline ChessColor GetColor() const { return color; }
    inline ChessRank GetRank() const { return rank; }
    void SetRank(ChessRank);
    inline int GetMoveCount() const { return moveCount; }
    inline void IncrementMoveCount() { moveCount++; }
    inline void DecrementMoveCount() { moveCount--; }
    inline int GetDelta() const { return color==WHITE ? -1 : 1; }
    inline int GetStartingRow() const { return color==WHITE ? 6 : 1; }
    QImage* GetImage() const;
private:
    const ChessColor color;
    ChessRank rank;
    int moveCount;
    QImage* image;
};

#endif // CHESSPIECE_H
