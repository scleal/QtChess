#ifndef CHESSCOLOR_H
#define CHESSCOLOR_H

enum ChessColor { WHITE, BLACK };

static ChessColor SwitchColor(ChessColor color) { return color == WHITE ? BLACK : WHITE; }

#endif // CHESSCOLOR_H
