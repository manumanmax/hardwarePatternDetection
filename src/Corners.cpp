/*
 * Corners.cpp
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#include "Corners.h"

Corners::Corners() {
	// TODO Auto-generated constructor stub

}

Corners::~Corners() {
	// TODO Auto-generated destructor stub
}

void Corners::print_corners() {
	std::cout << "Corners : " << std::endl << "(" << top_left.x << ","
			<< top_left.y << ")" << " - (" << top_right.x << "," << top_right.y
			<< ")" << std::endl << "(" << bot_left.x << "," << bot_left.y << ")"
			<< " - (" << bot_right.x << "," << bot_right.y << ")" << std::endl;
}

bool Corners::is_in(Point2d point) {
	if (point.x < top_left.x || point.x > top_right.x)
		return false;
	if (point.y < top_left.y || point.y > bot_right.y)
		return false;

	return true;
}

Point2f Corners::top_left_shift(const Point2f& shift) {
	return top_left + shift;
}
Point2f Corners::top_right_shift(const Point2f& shift) {
	return top_right + shift;
}
Point2f Corners::bot_left_shift(const Point2f& shift) {
	return bot_left + shift;
}
Point2f Corners::bot_right_shift(const Point2f& shift) {
	return bot_right + shift;
}

