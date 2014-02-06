#include "chesspiece.h"

const std::array<const char* const, 12> ChessPiece::fileNames = {
    ":/pieces/white_queen.png",
    ":/pieces/white_rook.png",
    ":/pieces/white_bishop.png",
    ":/pieces/white_knight.png",
    ":/pieces/white_king.png",
    ":/pieces/white_pawn.png",
    ":/pieces/black_queen.png",
    ":/pieces/black_rook.png",
    ":/pieces/black_bishop.png",
    ":/pieces/black_knight.png",
    ":/pieces/black_king.png",
    ":/pieces/black_pawn.png"
};

ChessPiece::ChessPiece(ChessRank rank, ChessColor color) :
    rank(rank),
    color(color),
    moveCount(0),
    image(new QImage(fileNames[rank+color])) {}

ChessPiece::ChessPiece(const ChessPiece& rhs) :
    rank(rhs.rank),
    color(rhs.color),
    moveCount(rhs.moveCount),
    image(NULL) {}

QImage* ChessPiece::GetImage() const{
    return image;
}

ChessPiece::~ChessPiece(){
    delete image;
}

void ChessPiece::SetRank(ChessRank newRank){
    rank = newRank;
    delete image;
    image = new QImage(fileNames[rank+color]);
}
