/*
 * Board.cpp
 *
 *      Author: Christof
 */

#include "Board.h"
#include <string>
#include <iostream>
using namespace std;

Board::Board() {
	// Set up initial board state
	for (int i = 0; i < sizeof(this->tiles) / sizeof(this->tiles[0]); i++) {
		for (int j = 0; j < sizeof(this->tiles) / sizeof(this->tiles[0]); j++) {
			this->tiles[i][j].setPiece(this->pieceNames[i][j], floor(i / 4));
		}
	}
	this->turnColor = 1;
	this->kingPos[0][0] = 3;
	this->kingPos[0][1] = 0;
	this->kingPos[1][0] = 3;
	this->kingPos[1][1] = 7;
}

Tile Board::getTile(int x, int y) {
	return this->tiles[x][y];
}

int Board::getTurnColor() {
	return this->turnColor;
}

// Moves piece from (x1, y1) to (x2, y2) if allowed and valid
bool Board::move(int x1, int y1, int x2, int y2) {
	if (x1 > 7 || y1 > 7) {
		return false;
	}
	if (this->tiles[y1][x1].getPiece() != 0) {
		if (this->checkMovement(x1, y1, x2, y2)) {
			int tempPiece = this->tiles[y2][x2].getPiece();
			int tempColor = this->tiles[y2][x2].getColor();
			this->tiles[y2][x2].setPiece(this->tiles[y1][x1].getPiece(), this->tiles[y1][x1].getColor());
			this->tiles[y1][x1].setPiece(0, 0);
			if (this->tiles[y2][x2].getPiece() == 7) {
				this->kingPos[this->turnColor][0] = x2;
				this->kingPos[this->turnColor][1] = y2;
			}
			if (this->checkCheck(this->kingPos[this->turnColor][0], this->kingPos[this->turnColor][1])) {
				if (this->tiles[y2][x2].getPiece() == 7) {
					this->kingPos[this->turnColor][0] = x1;
					this->kingPos[this->turnColor][1] = y1;
				}
				this->tiles[y1][x1].setPiece(this->tiles[y2][x2].getPiece(), this->tiles[y2][x2].getColor());
				this->tiles[y2][x2].setPiece(tempPiece, tempColor);
				return false;
			}
			if ((y2 == 7 && this->tiles[y2][x2].getPiece() == 2) || (y2 == 0 && this->tiles[y2][x2].getPiece() == 2)) {
				this->tiles[y2][x2].setPiece(6, this->tiles[y2][x2].getColor());
			}
			this->turnColor = abs(this->turnColor - 1);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// Checks if movement from (x1, y1) to (x2, y2) is allowed and valid
bool Board::checkMovement(int x1, int y1, int x2, int y2) {
	Tile tile = this->tiles[y1][x1];
	Tile nextTile = this->tiles[y2][x2];
	if (tile.getPiece() == 2) { // Pawn
		if (tile.getColor() == 0) {
			if ((x2 - x1 == 0) && (y2 - y1 == 1) && (nextTile.getPiece() == 0)) {
				return true;
			} else if ((x2 - x1 == 0) && (y2 - y1 == 2) && (nextTile.getPiece() == 0) && (y1 == 1)) {
				return true;
			} else if ((abs(x2 - x1) == 1) && (y2 - y1 == 1) && (nextTile.getPiece() > 0) && (nextTile.getColor() == 1)) {
				return true;
			}
		} else {
			if ((x1 - x2 == 0) && (y1 - y2 == 1) && (nextTile.getPiece() == 0)) {
				return true;
			} else if ((x1 - x2 == 0) && (y1 - y2 == 2) && (nextTile.getPiece() == 0) && (y1 == 6)) {
				return true;
			} else if ((abs(x1 - x2) == 1) && (y1 - y2 == 1) && (nextTile.getPiece() > 0) && (nextTile.getColor() == 0)) {
				return true;
			}
		}
	} else if (tile.getPiece() == 3) { // Rook
		if (x1 != x2 && y1 == y2) {
			if (x1 > x2) {
				for (int i = x1 - 1; i > x2; i--) {
					if (this->tiles[y1][i].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			} else {
				for (int i = x1 + 1; i < x2; i++) {
					if (this->tiles[y1][i].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			}
		} else if (x1 == x2 && y1 != y2) {
			if (y1 > y2) {
				for (int i = y1 - 1; i > y2; i--) {
					if (this->tiles[i][x1].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			} else {
				for (int i = y1 + 1; i < y2; i++) {
					if (this->tiles[i][x1].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			}
		}
	} else if (tile.getPiece() == 4) { // Knight
		if (abs(x2 - x1) == 2 && abs(y2 - y1) == 1 && (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor())) {
			return true;
		} else if (abs(x2 - x1) == 1 && abs(y2 - y1) == 2 && (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor())) {
			return true;
		}
	} else if (tile.getPiece() == 5) { // Bishop
		if (abs(x2 - x1) == abs(y2 - y1)) {
			for (int i = 1; i < abs(x2 - x1); i++) {
				if (x2 - x1 > 0) {
					if (y2 - y1 > 0) {
						if (this->tiles[y1 + i][x1 + i].getPiece() > 0) {
							return false;
						}
					} else {
						if (this->tiles[y1 - i][x1 + i].getPiece() > 0) {
							return false;
						}
					}
				} else {
					if (y2 - y1 > 0) {
						if (this->tiles[y1 + i][x1 - i].getPiece() > 0) {
							return false;
						}
					} else {
						if (this->tiles[y1 - i][x1 - i].getPiece() > 0) {
							return false;
						}
					}
				}
			}
			if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
				return true;
			}
		}
	} else if (tile.getPiece() == 6) { // Queen
		if (abs(x2 - x1) == abs(y2 - y1)) {
			for (int i = 1; i < abs(x2 - x1); i++) {
				if (x2 - x1 > 0) {
					if (y2 - y1 > 0) {
						if (this->tiles[y1 + i][x1 + i].getPiece() > 0) {
							return false;
						}
					} else {
						if (this->tiles[y1 - i][x1 + i].getPiece() > 0) {
							return false;
						}
					}
				} else {
					if (y2 - y1 > 0) {
						if (this->tiles[y1 + i][x1 - i].getPiece() > 0) {
							return false;
						}
					} else {
						if (this->tiles[y1 - i][x1 - i].getPiece() > 0) {
							return false;
						}
					}
				}
			}
			if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
				return true;
			}
		} else if (x1 != x2 && y1 == y2) {
			if (x1 > x2) {
				for (int i = x1 - 1; i > x2; i--) {
					if (this->tiles[y1][i].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			} else {
				for (int i = x1 + 1; i < x2; i++) {
					if (this->tiles[y1][i].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			}
		} else if (x1 == x2 && y1 != y2) {
			if (y1 > y2) {
				for (int i = y1 - 1; i > y2; i--) {
					if (this->tiles[i][x1].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			} else {
				for (int i = y1 + 1; i < y2; i++) {
					if (this->tiles[i][x1].getPiece() > 0) {
						return false;
					}
				}
				if (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor()) {
					return true;
				}
			}
		}
	} else if (tile.getPiece() == 7) { // King
		if (abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1 && (nextTile.getPiece() == 0 || nextTile.getColor() != tile.getColor())) {
			return true;
		}
	}
	return false;
}

// Checks if the king at tile (x, y) is in check
bool Board::checkCheck(int x, int y) {
	if (x == -1 && y == -1) {
		x = this->kingPos[1][0];
		y = this->kingPos[1][1];
	} else if (x == -2 && y == -2) {
		x = this->kingPos[0][0];
		y = this->kingPos[0][1];
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->tiles[j][i].getPiece() != 0 && this->tiles[j][i].getColor() != this->tiles[y][x].getColor() && this->checkMovement(i, j, x, y)) {
				return true;
			}
		}
	}
	return false;
}

// Finds all possible movement from tile (x, y)
void Board::possibleMovement(int x, int y) {
	Tile tile = this->tiles[y][x];
	if (tile.getPiece() == 0) { // Empty
		return;
	} else if (tile.getPiece() == 2) { // Pawn
		if (tile.getColor() == 0) {
			if (y == 1) {
				if (this->checkMovement(x, y, x, y + 2)) {
					this->setPossible(x, y, 0, 2);
				}
			}
			if (this->checkMovement(x, y, x, y + 1)) {
				this->setPossible(x, y, 0, 1);
			}
			if (this->checkMovement(x, y, x + 1, y + 1)) {
				this->setPossible(x, y, 1, 1);
			}
			if (this->checkMovement(x, y, x - 1, y + 1)) {
				this->setPossible(x, y, -1, 1);
			}
		} else {
			if (y == 6) {
				if (this->checkMovement(x, y, x, y - 2)) {
					this->setPossible(x, y, 0, -2);
				}
			}
			if (this->checkMovement(x, y, x, y - 1)) {
				this->setPossible(x, y, 0, -1);
			}
			if (this->checkMovement(x, y, x + 1, y - 1)) {
				this->setPossible(x, y, 1, -1);
			}
			if (this->checkMovement(x, y, x - 1, y - 1)) {
				this->setPossible(x, y, -1, -1);
			}
		}
	} else if (tile.getPiece() == 3) { // Rook
		for (int i = -1; i < 2; i += 2) {
			bool stillValid = true;
			int count = 1;
			while (stillValid) {
				if (x + (i * count) >= 0 && x + (i * count) < 8) {
					if (this->checkMovement(x, y, x + (i * count), y)) {
						this->setPossible(x, y, (i * count), 0);
					}
					count++;
				} else {
					stillValid = false;
				}
			}
		}
		for (int j = -1; j < 2; j += 2) {
			bool stillValid = true;
			int count = 1;
			while (stillValid) {
				if (y + (j * count) >= 0 && y + (j * count) < 8) {
					if (this->checkMovement(x, y, x, y + (j * count))) {
						this->setPossible(x, y, 0, (j * count));
					}
					count++;
				} else {
					stillValid = false;
				}
			}
		}
	} else if (tile.getPiece() == 4) { // Knight
		int j = 2;
		for (int i = 1; i < 3; i++) {
			if (y + j >= 0 && y + j < 8 && x + i >= 0 && x + i < 8) {
				if (this->checkMovement(x, y, x + i, y + j)) {
					this->setPossible(x, y, i, j);
				}
			}
			if (y - j >= 0 && y - j < 8 && x + i >= 0 && x + i < 8) {
				if (this->checkMovement(x, y, x + i, y - j)) {
					this->setPossible(x, y, i, -j);
				}
			}
			j--;
		}
		j = 2;
		for (int i = 1; i < 3; i++) {
			if (y + j >= 0 && y + j < 8 && x - i >= 0 && x - i < 8) {
				if (this->checkMovement(x, y, x - i, y + j)) {
					this->setPossible(x, y, -i, j);
				}
			}
			if (y - j >= 0 && y - j < 8 && x - i >= 0 && x - i < 8) {
				if (this->checkMovement(x, y, x - i, y - j)) {
					this->setPossible(x, y, -i, -j);
				}
			}
			j--;
		}
	} else if (tile.getPiece() == 5) { // Bishop
		for (int i = -1; i < 2; i+=2) {
			for (int j = -1; j < 2; j += 2) {
				bool stillValid = true;
				int count = 1;
				while (stillValid) {
					if (y + (j * count) >= 0 && y + (j * count) < 8 && x + (i * count) >= 0 && x + (i * count) < 8) {
						if (this->checkMovement(x, y, x + (i * count), y + (j * count))) {
							this->setPossible(x, y, (i * count), (j * count));
						}
						count++;
					} else {
						stillValid = false;
					}
				}
			}
		}
	} else if (tile.getPiece() == 6) { // Queen
		for (int i = -1; i < 2; i += 2) {
			bool stillValid = true;
			int count = 1;
			while (stillValid) {
				if (x + (i * count) >= 0 && x + (i * count) < 8) {
					if (this->checkMovement(x, y, x + (i * count), y)) {
						this->setPossible(x, y, (i * count), 0);
					}
					count++;
				} else {
					stillValid = false;
				}
			}
		}
		for (int j = -1; j < 2; j += 2) {
			bool stillValid = true;
			int count = 1;
			while (stillValid) {
				if (y + (j * count) >= 0 && y + (j * count) < 8) {
					if (this->checkMovement(x, y, x, y + (j * count))) {
						this->setPossible(x, y, 0, (j * count));
					}
					count++;
				} else {
					stillValid = false;
				}
			}
		}
		for (int i = -1; i < 2; i += 2) {
			for (int j = -1; j < 2; j += 2) {
				bool stillValid = true;
				int count = 1;
				while (stillValid) {
					if (y + (j * count) >= 0 && y + (j * count) < 8 && x + (i * count) >= 0 && x + (i * count) < 8) {
						if (this->checkMovement(x, y, x + (i * count), y + (j * count))) {
							this->setPossible(x, y, (i * count), (j * count));
						}
						count++;
					} else {
						stillValid = false;
					}
				}
			}
		}
	} else if (tile.getPiece() == 7) { // King
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (y + j >= 0 && y + j < 8 && x + i >= 0 && x + i < 8) {
					if (this->checkMovement(x, y, x + i, y + j)) {
						this->setPossible(x, y, i, j);
					}
				}
			}
		}
	}
}

// Sets possible tile (x + i, y + j) of movement for piece on tile (x, y)
void Board::setPossible(int x, int y, int i, int j) {
	int tempPiece = this->tiles[y + j][x + i].getPiece();
	int tempColor = this->tiles[y + j][x + i].getColor();
	this->tiles[y + j][x + i].setPiece(this->tiles[y][x].getPiece(), this->tiles[y][x].getColor());
	this->tiles[y][x].setPiece(0, 0);
	if (this->tiles[y + j][x + i].getPiece() == 7) {
		if (!this->checkCheck(x + i, y + j)) {
			this->tiles[y + j][x + i].setSelected(1);
		}
	} else {
		if (!this->checkCheck(this->kingPos[this->turnColor][0], this->kingPos[this->turnColor][1])) {
			this->tiles[y + j][x + i].setSelected(1);
		}
	}
	this->tiles[y][x].setPiece(this->tiles[y + j][x + i].getPiece(), this->tiles[y + j][x + i].getColor());
	this->tiles[y + j][x + i].setPiece(tempPiece, tempColor);
}

// Clears all selections on the board
void Board::clearPossible() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->tiles[j][i].setSelected(0);
		}
	}
}

// Checks if a checkmate or draw scenario has occurred
bool Board::checkWin(int turnColor) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->tiles[j][i].getPiece() != 0 && this->tiles[j][i].getColor() == turnColor) {
				this->possibleMovement(i, j);
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->tiles[j][i].getSelected() == 1) {
				return false;
			}
		}
	}
	return true;
}

// Sets winning Kings to green and losing King to red
void Board::setWin() {
	this->tiles[this->kingPos[this->turnColor][1]][this->kingPos[this->turnColor][0]].setSelected(-2);
	this->tiles[this->kingPos[abs(this->turnColor - 1)][1]][this->kingPos[abs(this->turnColor - 1)][0]].setSelected(-1);
}

// Sets both Kings to red
void Board::setDraw() {
	this->tiles[this->kingPos[this->turnColor][1]][this->kingPos[this->turnColor][0]].setSelected(-2);
	this->tiles[this->kingPos[abs(this->turnColor - 1)][1]][this->kingPos[abs(this->turnColor - 1)][0]].setSelected(-2);
}