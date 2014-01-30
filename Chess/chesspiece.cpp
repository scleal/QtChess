#include "chesspiece.h"

ChessPiece::ChessPiece(ChessRank rank, ChessColor color, const char* fileName) :
    rank(rank),
    color(color),
    moveCount(0),
    image(new QImage(fileName)){
}

ChessPiece::ChessPiece(const ChessPiece& rhs) :
    rank(rhs.rank),
    color(rhs.color),
    moveCount(rhs.moveCount),
    image(NULL)
{}

ChessPiece::~ChessPiece(){
    if (image) delete image;
}

QImage* ChessPiece::GetImage() const{
    return image;
}
