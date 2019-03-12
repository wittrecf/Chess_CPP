/*
 * Board.h
 *
 *      Author: Christof
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "Tile.h"
#include <string>
#include <iostream>
using namespace std;

class Board {
public:
	Board();
	int pieceNames[8][8] = {
		{ 3, 4, 5, 7, 6, 5, 4, 3 },
		{ 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 3, 4, 5, 7, 6, 5, 4, 3 }
	};
	Tile getTile(int x, int y);
	int getTurnColor();
	bool move(int x1, int y1, int x2, int y2);
	bool checkMovement(int x1, int y1, int x2, int y2);
	bool checkCheck(int x, int y);
	void possibleMovement(int x, int y);
	void clearPossible();
	bool checkWin(int turnColor);
	void setWin();
	void setDraw();
private:
	Tile tiles[8][8];
	int turnColor;
	int kingPos[2][2];
	void setPossible(int x, int y, int i, int j);
};

#endif /* BOARD_H_ */
