/*
 * Tile.h
 *
 *      Author: Christof
 */

#ifndef TILE_H_
#define TILE_H_
#include <string>
#include <iostream>
using namespace std;

class Tile {
public:
	Tile();
	void setPiece(int p, int color);
	int getPiece();
	int getColor();
	void setSelected(int s);
	int getSelected();
private:
	int piece;
	int color;
	int selected;
};

#endif /* TILE_H_ */
