#ifndef CHESSCOLOR_H
#define CHESSCOLOR_H

enum ChessColor { WHITE=0, BLACK=6 };

static ChessColor SwitchColor(ChessColor color) { return color == WHITE ? BLACK : WHITE; }

#endif // CHESSCOLOR_H
