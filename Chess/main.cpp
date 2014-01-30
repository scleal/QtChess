#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    ChessBoard board;
    MainWindow w(board);
    w.setFixedSize(480,480);
    w.show();
    return a.exec();
}
