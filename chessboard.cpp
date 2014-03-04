#include <string.h>
#include "chessboard.h"

ChessBoard::ChessBoard(){
    InitializeBoard();
}

ChessBoard::ChessBoard(const ChessBoard& rhs){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (ChessPiece* piece = rhs.GetPiece(i,j)){
                chessBoard[i][j] = new ChessPiece(*piece);
            } else {
                chessBoard[i][j] = NULL;
            }
        }
    }
}

ChessBoard::~ChessBoard(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            ChessPiece* piece = GetPiece(i,j);
            delete piece;
        }
    }
}

ChessPiece* ChessBoard::GetPiece(int x, int y) const{
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return NULL;
    return chessBoard[x][y];
}

void ChessBoard::InitializeBoard(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            chessBoard[i][j] = NULL;
        }
    }
    for (int i = 0; i < 8; i++){
        chessBoard[i][6] = new ChessPiece(PAWN, WHITE);
        chessBoard[i][1] = new ChessPiece(PAWN, BLACK);
    }
    chessBoard[0][7] = new ChessPiece(ROOK,   WHITE);
    chessBoard[1][7] = new ChessPiece(KNIGHT, WHITE);
    chessBoard[2][7] = new ChessPiece(BISHOP, WHITE);
    chessBoard[3][7] = new ChessPiece(QUEEN,  WHITE);
    chessBoard[4][7] = new ChessPiece(KING,   WHITE);
    chessBoard[5][7] = new ChessPiece(BISHOP, WHITE);
    chessBoard[6][7] = new ChessPiece(KNIGHT, WHITE);
    chessBoard[7][7] = new ChessPiece(ROOK,   WHITE);

    chessBoard[0][0] = new ChessPiece(ROOK,   BLACK);
    chessBoard[1][0] = new ChessPiece(KNIGHT, BLACK);
    chessBoard[2][0] = new ChessPiece(BISHOP, BLACK);
    chessBoard[3][0] = new ChessPiece(QUEEN,  BLACK);
    chessBoard[4][0] = new ChessPiece(KING,   BLACK);
    chessBoard[5][0] = new ChessPiece(BISHOP, BLACK);
    chessBoard[6][0] = new ChessPiece(KNIGHT, BLACK);
    chessBoard[7][0] = new ChessPiece(ROOK,   BLACK);
}

ChessMove* ChessBoard::GetKingPosition(ChessColor currentPlayer) const{
    int i = 0, j = 0;
    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            ChessPiece* curPiece = GetPiece(i,j);
            if (curPiece && curPiece->GetRank()==KING && curPiece->GetColor() == currentPlayer){
                goto done;
            }
        }
    }
    done:
    return new ChessMove(i,j,-1,-1);
}

inline void ChessBoard::MovePiece(ChessPiece* piece, int x, int y){
    chessBoard[x][y] = piece;
}

inline void ChessBoard::ClearCell(int x, int y){
    chessBoard[x][y] = NULL;
}

ChessBoard* ChessBoard::DoMove(const ChessMove& move){
    ChessPiece* piece = GetPiece(move.startX, move.startY);
    ChessRank rank = piece->GetRank();
    ChessColor color = piece->GetColor();
    if (rank==KING && move.IsValidCastleMove(*this)){
        int x = 7;
        int deltaX = 1;
        if (move.endX == move.startX-2){
            x = 0;
            deltaX = -1;
        }
        MovePiece(GetPiece(x, move.startY), move.startX+deltaX, move.startY);
        ClearCell(x, move.startY);
    } else if (rank == PAWN && move.IsValidEnPassantMove(*this, GetDelta(color), GetStartingRow(color))){
        ChessPiece* occupyingPiece = GetPiece(move.endX, move.startY);
        delete occupyingPiece;
        ClearCell(move.endX, move.startY);
    }
    piece->IncrementMoveCount();
    ChessPiece* occupyingPiece = GetPiece(move.endX, move.endY);
    delete occupyingPiece;
    MovePiece(piece, move.endX, move.endY);
    ClearCell(move.startX, move.startY);
    return this;
}

bool ChessBoard::UnderAttack(ChessColor opposingPlayer, int x, int y) const{
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (GetPiece(i, j) && GetPiece(i, j)->GetColor()==opposingPlayer){
                ChessMove move(i,j,x,y);
                if (move.IsValidMove(*this))
                    return true;
            }
        }
    }
    return false;
}

bool ChessBoard::InCheck(ChessColor currentPlayer) const{
    ChessMove* kingPosition = GetKingPosition(currentPlayer);
    ChessColor opposingPlayer = SwitchColor(currentPlayer);
    bool result = UnderAttack(opposingPlayer, kingPosition->startX, kingPosition->startY);
    delete kingPosition;
    return result;
}

bool ChessBoard::NoValidMoves(ChessColor currentPlayer){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (GetPiece(i, j) && GetPiece(i, j)->GetColor()==currentPlayer){
                //clear psuedo-legal moves vector
                psuedoLegalMoves.clear();
                //fill psuedo-legal moves vector
                GeneratePsuedoLegalMoves(i,j);
                //check if psuedo legal moves are actually legal
                for (std::vector<ChessMove*>::iterator it = psuedoLegalMoves.begin(); it != psuedoLegalMoves.end(); it++){
                    if ((*it)->IsValidMove(*this) && !(*it)->PutsInCheck(*this))
                        return false;
                }
            }
        }
    }
    return true;
}

void ChessBoard::GeneratePsuedoLegalMoves(int x, int y){
    switch (GetPiece(x,y)->GetRank()){
    case PAWN:
        return GeneratePLPawnMoves(x,y);
    case ROOK:
        return GeneratePLRookMoves(x,y);
    case KNIGHT:
        return GeneratePLKnightMoves(x,y);
    case BISHOP:
        return GeneratePLBishopMoves(x,y);
    case QUEEN:
        return GeneratePLQueenMoves(x,y);
    case KING:
        return GeneratePLKingMoves(x,y);
    }
}

void ChessBoard::GeneratePLPawnMoves(int startX, int startY){
    int delta = GetDelta(GetPiece(startX, startY)->GetColor());
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX-1, startY+delta));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX, startY+delta));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX, startY+2*delta));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX+1, startY+delta));
}

void ChessBoard::GeneratePLRookMoves(int startX, int startY){
    for (int i = 0; i < 8; i++){
        if (i!=startY)
            psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX, i));
        if (i!=startX)
            psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, startY));
    }
}

void ChessBoard::GeneratePLKnightMoves(int startX, int startY){
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX+1, startY+2));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX+1, startY-2));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX+2, startY+1));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX+2, startY-1));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX-1, startY+2));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX-1, startY-2));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX-2, startY+1));
    psuedoLegalMoves.push_back(new ChessMove(startX, startY, startX-2, startY-1));
}

void ChessBoard::GeneratePLBishopMoves(int startX, int startY){
    int i, j;
    for (i = startX+1, j = startY+1; i < 8 && j < 8; i++, j++)
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, j));
    for (i = startX+1, j = startY-1; i < 8 && j >= 0; i++, j--)
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, j));
    for (i = startX-1, j = startY+1; i >= 0 && j < 8; i--, j++)
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, j));
    for (i = startX-1, j = startY-1; i >= 0 && j >= 0; i--, j--)
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, j));
}

void ChessBoard::GeneratePLQueenMoves(int startX, int startY){
    GeneratePLRookMoves(startX, startY);
    GeneratePLBishopMoves(startX, startY);
}

void ChessBoard::GeneratePLKingMoves(int startX, int startY){
    //current row
    for (int i = startX-2; i <= startX+2; i++)
        if (i!=startX)
            psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, startY));
    for (int i = startX-1; i <= startX+1; i++){
        //below row
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, startY+1));
        //above row
        psuedoLegalMoves.push_back(new ChessMove(startX, startY, i, startY-1));
    }
}
