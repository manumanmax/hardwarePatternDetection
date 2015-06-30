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

int max2(int v1, int v2) {
	if (v1 > v2)
		return v1;
	return v2;
}

int max4(int v1, int v2, int v3, int v4) {
	int max = max2(v1, v2);
	max = max2(max, v3);
	max = max2(max, v4);
	return max;
}

int min2(int v1, int v2) {
	if (v1 < v2)
		return v1;
	return v2;
}

int min4(int v1, int v2, int v3, int v4) {
	int min = min2(v1, v2);
	min = min2(min, v3);
	min = min2(min, v4);
	return min;
}

//bounding diamond
bool Corners::is_in(Point2d point) {

	int p1, p2, p3, p4, q1, q2, q3, q4;
	p1 = top_left.x + top_left.y;
	q1 = top_left.x - top_left.y;

	p2 = top_right.x + top_right.y;
	q2 = top_right.x - top_right.y;

	p3 = bot_right.x + bot_right.y;
	q3 = bot_right.x - bot_right.y;

	p4 = bot_left.x + bot_left.y;
	q4 = bot_left.x - bot_left.y;

	int pmax = max4(p1, p2, p3, p4);
	int qmax = max4(q1, q2, q3, q4);

	int pmin = min4(p1, p2, p3, p4);
	int qmin = min4(q1, q2, q3, q4);

	if ((point.x + point.y > pmin && point.x + point.y < pmax) &&
		(point.x - point.y > qmin && point.x - point.y < qmax)	){
		return true;
	}
	return false;
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

