#include "Point.h"

#include "Globals.h"
#include <cstdlib>

using namespace std;

/* constructors/destructor */
Point::Point(int row, int col) : _row(row), _col(col) {
}

/* methods */
int Point::col() const {
	return _col;
}

int Point::col(int col) {
	_col = col;
	return this->col();
}

int Point::row() const {
	return _row;
}

int Point::row(int row) {
	_row = row;
	return this->row();
}

bool Point::insideMap() const {
	return row() >= MAP_ROW_BEGIN && row() <= MAP_ROW_END && col() >= MAP_COL_BEGIN && col() <= MAP_COL_END;
}

Point& Point::moveDirection(unsigned char direction) {
	switch (direction) {
	case NW:
		return moveNorthwest();

	case N:
		return moveNorth();

	case NE:
		return moveNortheast();

	case E:
		return moveEast();

	case SE:
		return moveSoutheast();

	case S:
		return moveSouth();

	case SW:
		return moveSouthwest();

	case W:
		return moveWest();

	default:
		return *this;
	}
}

Point& Point::moveNorthwest() {
	--_row;
	--_col;
	return *this;
}

Point& Point::moveNorth() {
	--_row;
	return *this;
}

Point& Point::moveNortheast() {
	--_row;
	++_col;
	return *this;
}

Point& Point::moveEast() {
	++_col;
	return *this;
}

Point& Point::moveSoutheast() {
	++_row;
	++_col;
	return *this;
}

Point& Point::moveSouth() {
	++_row;
	return *this;
}

Point& Point::moveSouthwest() {
	++_row;
	--_col;
	return *this;
}

Point& Point::moveWest() {
	--_col;
	return *this;
}

int Point::gridDistance(const Point& p1, const Point& p2) {
	return max(abs(p1.row() - p2.row()), abs(p1.col() - p2.col()));
}

/* operator overloading */
bool Point::operator<(const Point& p) const {
	return (_row > p._row || (_row == p._row && _col > p._col));
}

bool Point::operator>(const Point& p) const {
	return (_row > p._row || (_row == p._row && _col > p._col));
}

bool Point::operator<=(const Point& p) const {
	return (_row < p._row || (_row == p._row && _col <= p._col));
}

bool Point::operator>=(const Point& p) const {
	return (_row > p._row || (_row == p._row && _col >= p._col));
}

bool Point::operator==(const Point& p) const {
	return (_row == p._row && _col == p._col);
}

bool Point::operator!=(const Point& p) const {
	return (_row != p._row || _col != p._col);
}

ostream& operator<<(ostream& os, const Point& p) {
	return os << "(" << p.row() << ", " << p.col() << ")";
}
