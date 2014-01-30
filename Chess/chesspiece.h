#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <array>
#include <QImage>
#include "chessrank.h"
#include "chesscolor.h"

class ChessPiece{
public:
    ChessPiece(ChessRank, ChessColor);
    ChessPiece(const ChessPiece&);
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
    const static std::array<const char* const, 12> fileNames;
};

#endif // CHESSPIECE_H
