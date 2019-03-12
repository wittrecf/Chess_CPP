/*
 * Tile.cpp
 *
 *      Author: Christof
 */

#include "Tile.h"

Tile::Tile() {
	this->selected = 0;
}

void Tile::setPiece(int p, int c) {
	this->piece = p;
	this->color = c;
}

int Tile::getPiece() {
	return this->piece;
}

int Tile::getColor() {
	return this->color;
}

void Tile::setSelected(int s) {
	this->selected = s;
}

int Tile::getSelected() {
	return this->selected;
}