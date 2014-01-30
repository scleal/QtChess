#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QWidget>

#include "chessboard.h"
class ChessBoard;

class MainWindow : public QWidget{
    Q_OBJECT
public:
    MainWindow(ChessBoard&);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void DrawBoard(QPainter&);
    void DrawPossibleMoves(QPainter&);
    void DrawStatus(QPainter&);
    bool IsValidSelection(ChessPiece*);
    void MakeSelection(int, int);
    void SwitchTurns();
private:
    ChessBoard& board;
    ChessColor currentPlayer;
    ChessPiece* startPiece;
    int startX, startY;
    bool promotion;
    const int CELL_SIZE = 60;
};

#endif // MAINWINDOW_H
