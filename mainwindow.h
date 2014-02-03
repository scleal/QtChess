#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>

#include "chessboard.h"
class ChessBoard;

class MainWindow : public QWidget{
    Q_OBJECT
public:
    MainWindow(ChessBoard&);
public slots:
    void Promote(int);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void DrawBoard(QPainter&);
    void DrawPossibleMoves(QPainter&);
    void DrawStatus(QPainter&);
    bool IsValidSelection(ChessPiece*);
    void MakeSelection(int, int);
    void SwitchTurns();
    void InitializeLayout();
private:
    ChessBoard& board;
    ChessColor currentPlayer;
    ChessPiece* startPiece;
    int startX, startY;
    bool promotion, gameOver;
    const int CELL_SIZE = 60;
    QWidget* promotions;
    QHBoxLayout* layout;
    QSignalMapper* mapper;
    std::array<QPushButton*, 4> buttons;
};

#endif // MAINWINDOW_H
