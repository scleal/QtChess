#include <cstdlib>
#include "chessmove.h"

ChessMove::ChessMove(const int startX, const int startY, const int endX, const int endY) :
    startX(startX),
    startY(startY),
    endX(endX),
    endY(endY)
{}

bool ChessMove::IsValidMove(const ChessBoard& board) const{
    ChessPiece* startPiece = board.GetPiece(startX, startY);
    ChessPiece* occupyingPiece = board.GetPiece(endX, endY);
    if (occupyingPiece && occupyingPiece->GetColor() == startPiece->GetColor())
        return false;
    switch (startPiece->GetRank()){
    case PAWN:
        return IsValidMovePawn(board) || IsValidEnPassantMove(board, startPiece->GetDelta(), startPiece->GetStartingRow());
    case ROOK:
        return IsValidMoveRook(board);
    case KNIGHT:
        return IsValidMoveKnight();
    case BISHOP:
        return IsValidMoveBishop(board);
    case QUEEN:
        return IsValidMoveBishop(board) || IsValidMoveRook(board);
    case KING:
        return IsValidMoveKing() || IsValidCastleMove(board);
    }
    return false;
}

bool ChessMove::PutsInCheck(const ChessBoard& board) const{
    ChessBoard boardCopy(board);
    ChessPiece* pieceCopy(board.GetPiece(startX,startY));
    return (boardCopy.DoMove(*this))->InCheck(pieceCopy->GetColor());
}

bool ChessMove::IsValidCastleMove(const ChessBoard& board) const{
    ChessPiece* king = board.GetPiece(startX, startY);
    if (king->GetMoveCount() != 0) return false;
    if (abs(startX-endX) != 2 || startY != endY) return false;
    int x = 7;
    int delta = 1;
    if (startX > endX){
        x = 0;
        delta = -1;
    }
    ChessPiece* rook = board.GetPiece(x, startY);
    if (rook && rook->GetMoveCount() != 0) return false;
    ChessColor opposingColor = SwitchColor(king->GetColor());
    for (int i = startX; GetCondition(i, startX, endX); i += delta){
        if ((board.GetPiece(i, startY) && i != startX)
          || board.UnderAttack(opposingColor,i,startY)) return false;
    }
    return true;
}

bool ChessMove::IsValidEnPassantMove(const ChessBoard& board, int delta, int startingRow) const{
    if (startY != startingRow+delta*3 || abs(startX-endX) != 1 || startY+delta != endY) return false;
    if (board.GetPiece(endX, endY)) return false;
    ChessPiece* potentialPawn = board.GetPiece(endX, startY);
    return potentialPawn && potentialPawn->GetRank()==PAWN && potentialPawn->GetMoveCount()==1;
}

bool ChessMove::GetCondition(int i, int start, int end){
    return (end < start) ? (i > end) : (i < end);
}

bool ChessMove::IsValidMovePawn(const ChessBoard& board) const{
    int delta = board.GetPiece(startX, startY)->GetDelta();
    int startingRow = board.GetPiece(startX, startY)->GetStartingRow();
    ChessPiece* occupyingPiece = board.GetPiece(endX, endY);
    if (occupyingPiece){
        return abs(startX-endX)==1 && startY+delta==endY;
    } else if (endX==startX){
        if (startingRow==startY){
            return endY==startingRow+delta || (!board.GetPiece(endX, startingRow+delta) && endY==startingRow+delta*2);
        } else {
            return endY==startY+delta;
        }
    }
    return false;
}

bool ChessMove::IsValidMoveRook(const ChessBoard& board) const{
    if (endX==startX){
        int deltaY = 1;
        if (startY > endY){deltaY = -1;}
        for (int i = startY+deltaY; GetCondition(i, startY, endY); i+=deltaY)
            if (board.GetPiece(startX, i)) return false;
        return true;
    } else if (endY==startY){
        int deltaX = 1;
        if (startX > endX){deltaX = -1;}
        for (int i = startX+deltaX; GetCondition(i, startX, endX); i+=deltaX)
            if (board.GetPiece(i, startY)) return false;
        return true;
    }
    return false;
}

bool ChessMove::GetConditionBishop(int i, int j) const{
    if (endX > startX && endY > startY)
        return i < endX && j < endY;
    if (endX < startX && endY > startY)
        return i > endX && j < endY;
    if (endX < startX && endY < startY)
        return i > endX && j > endY;
    return i < endX && j > endY;
}

bool ChessMove::IsValidMoveBishop(const ChessBoard & board) const{
    int i = -1, j = -1;
    int deltaX = 1, deltaY = 1;
    if (endX < startX && endY > startY){deltaX = -1;}
    else if (endX > startX && endY < startY){deltaY = -1;}
    else if (endX < startX && endY < startY){deltaX = -1; deltaY = -1;}
    for (i = startX+deltaX, j = startY+deltaY; GetConditionBishop(i,j); i+=deltaX, j+=deltaY)
        if (board.GetPiece(i, j)) return false;
    return i==endX && j==endY;
}

inline bool ChessMove::IsValidMoveKing() const{
    return abs(startX-endX) <= 1 && abs(startY-endY) <= 1;
}

inline bool ChessMove::IsValidMoveKnight() const{
    return (abs(endX-startX)==1 && abs(endY-startY)==2)
        || (abs(endY-startY)==1 && abs(endX-startX)==2);
}