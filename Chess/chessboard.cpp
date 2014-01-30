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

ChessPiece* ChessBoard::GetPiece(int x, int y) const
{
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
        chessBoard[i][6] = new ChessPiece(PAWN, WHITE, ":/pieces/white_pawn.png");
        chessBoard[i][1] = new ChessPiece(PAWN, BLACK, ":/pieces/black_pawn.png");
    }
    chessBoard[0][7] = new ChessPiece(ROOK,   WHITE, ":/pieces/white_rook.png");
    chessBoard[1][7] = new ChessPiece(KNIGHT, WHITE, ":/pieces/white_knight.png");
    chessBoard[2][7] = new ChessPiece(BISHOP, WHITE, ":/pieces/white_bishop.png");
    chessBoard[3][7] = new ChessPiece(QUEEN,  WHITE, ":/pieces/white_queen.png");
    chessBoard[4][7] = new ChessPiece(KING,   WHITE, ":/pieces/white_king.png");
    chessBoard[5][7] = new ChessPiece(BISHOP, WHITE, ":/pieces/white_bishop.png");
    chessBoard[6][7] = new ChessPiece(KNIGHT, WHITE, ":/pieces/white_knight.png");
    chessBoard[7][7] = new ChessPiece(ROOK,   WHITE, ":/pieces/white_rook.png");

    chessBoard[0][0] = new ChessPiece(ROOK,   BLACK, ":/pieces/black_rook.png");
    chessBoard[1][0] = new ChessPiece(KNIGHT, BLACK, ":/pieces/black_knight.png");
    chessBoard[2][0] = new ChessPiece(BISHOP, BLACK, ":/pieces/black_bishop.png");
    chessBoard[3][0] = new ChessPiece(QUEEN,  BLACK, ":/pieces/black_queen.png");
    chessBoard[4][0] = new ChessPiece(KING,   BLACK, ":/pieces/black_king.png");
    chessBoard[5][0] = new ChessPiece(BISHOP, BLACK, ":/pieces/black_bishop.png");
    chessBoard[6][0] = new ChessPiece(KNIGHT, BLACK, ":/pieces/black_knight.png");
    chessBoard[7][0] = new ChessPiece(ROOK,   BLACK, ":/pieces/black_rook.png");
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

ChessBoard* ChessBoard::DoMove(ChessPiece* piece, const ChessMove& move){
    ChessRank rank = piece->GetRank();
    if (rank==KING && move.IsValidCastleMove(*this)){
        int x = 7;
        int deltaX = 1;
        if (move.endX == move.startX-2){
            x = 0;
            deltaX = -1;
        }
        MovePiece(GetPiece(x, move.startY), move.startX+deltaX, move.startY);
        ClearCell(x, move.startY);
    } else if (rank == PAWN && move.IsValidEnPassantMove(*this, piece->GetDelta(), piece->GetStartingRow())){
        ClearCell(move.endX, move.startY);
    }
    piece->IncrementMoveCount();
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

bool ChessBoard::NoValidMoves(ChessColor currentPlayer) const{
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (GetPiece(i,j) && GetPiece(i, j)->GetColor()==currentPlayer){
                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        ChessMove move(i,j,k,l);
                        if (move.IsValidMove(*this) && !move.PutsInCheck(*this, GetPiece(i,j)))
                            return false;
                    }
                }
            }
        }
    }
    return true;
}
