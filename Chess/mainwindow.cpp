#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ChessBoard& board) :
    board(board)
{
    startPiece = NULL;
    startX = 0;
    startY = 0;
    currentPlayer = WHITE;
    promotion = false;
}

void MainWindow::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    DrawBoard(painter);
    if (!promotion && startPiece){
        QPen pen(Qt::red);
        painter.setPen(pen);
        painter.drawRect(startX*CELL_SIZE, startY*CELL_SIZE, CELL_SIZE, CELL_SIZE);
        DrawPossibleMoves(painter);
    }
    DrawStatus(painter);
}

void MainWindow::DrawBoard(QPainter& painter){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (i%2==j%2) painter.fillRect(i*60, j*60, 60, 60, Qt::white);
            else  painter.fillRect(i*60, j*60, 60, 60, Qt::gray);
            if (board.GetPiece(i,j)){
                painter.drawImage(i*60, j*60, *(board.GetPiece(i,j)->GetImage()));
            }
        }
    }
}

void MainWindow::DrawStatus(QPainter& painter){
    bool noValidMoves = board.NoValidMoves(currentPlayer);
    bool inCheck = board.InCheck(currentPlayer);
    QPen pen(Qt::red);
    QString curPlayer = currentPlayer == WHITE ? "WHITE" : "BLACK";
    if (noValidMoves && inCheck){
        painter.drawText(200, 200, curPlayer.append(" IN CHECKMATE"));
        QString winner = currentPlayer == WHITE ? "BLACK" : "WHITE";
        painter.drawText(200, 220, winner.append(" WINS"));
    } else if (noValidMoves){
        painter.drawText(200, 200, "STALEMATE");
    } else if (inCheck){
        painter.drawText(200, 200, curPlayer.append(" IN CHECK"));
    } else if (promotion){
        painter.drawText(200, 200, "Queen  (Q)");
        painter.drawText(200, 220, "Rook   (R)");
        painter.drawText(200, 240, "Bishop (B)");
        painter.drawText(200, 260, "Knight (K)");
    }
}

void MainWindow::DrawPossibleMoves(QPainter& painter){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            ChessMove move(startX, startY, i, j);
            if (move.IsValidMove(board) && !move.PutsInCheck(board)){
                QPen pen(Qt::blue);
                painter.setPen(pen);
                painter.drawRect(i*60, j*60, 60, 60);
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    if (!promotion){
        int x = (event->x())/60;
        int y = (event->y())/60;
        ChessMove move(startX, startY, x, y);
        if (IsValidSelection(startPiece) && move.IsValidMove(board) && !move.PutsInCheck(board)){
            board.DoMove(move);
            if (startPiece->GetRank()==PAWN && move.endY%7==0){
                promotion = true;
            }
            if (!promotion) SwitchTurns();
        } else if (IsValidSelection(board.GetPiece(x,y))){
            MakeSelection(x,y);
        }
    }
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if (promotion){
        int key = event->key();
        if (key==Qt::Key_Q){
            startPiece->SetRank(QUEEN);
            promotion = false;
        } else if (key==Qt::Key_R){
            startPiece->SetRank(ROOK);
            promotion = false;
        } else if (key==Qt::Key_B){
            startPiece->SetRank(BISHOP);
            promotion = false;
        } else if (key==Qt::Key_K){
            startPiece->SetRank(KNIGHT);
            promotion = false;
        }
        if (!promotion){
            SwitchTurns();
            repaint();
        }
    }
}

void MainWindow::SwitchTurns(){
    startPiece = NULL;
    currentPlayer = SwitchColor(currentPlayer);
}

bool MainWindow::IsValidSelection(ChessPiece* piece){
    return piece && piece->GetColor()==currentPlayer;
}

void MainWindow::MakeSelection(int x, int y){
    startPiece = board.GetPiece(x,y);
    startX = x;
    startY = y;
}
