#ifndef CHESSCOLOR_H
#define CHESSCOLOR_H

enum ChessColor { WHITE=0, BLACK=6 };

static inline ChessColor SwitchColor(ChessColor color) { return color == WHITE ? BLACK : WHITE; }
static inline int GetDelta(ChessColor color) { return color==WHITE ? -1 : 1; }
static inline int GetStartingRow(ChessColor color) { return color==WHITE ? 6 : 1; }

#endif // CHESSCOLOR_H
