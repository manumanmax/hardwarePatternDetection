/*
 * Corners.h
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#ifndef SRC_CORNERS_H_
#define SRC_CORNERS_H_
#include <cv.hpp>
#include "opencv2/core/core.hpp"
using namespace cv;
using namespace std;

class Corners {
public:
	Corners();
	Corners(Point2f top_left, Point2f top_right, Point2f bot_right,
			Point2f bot_left) :
			top_left(top_left), top_right(top_right), bot_right(bot_right), bot_left(
					bot_left) {
	}
	virtual ~Corners();
	void print_corners();
	bool is_in(Point2d point);

	Point2f top_left_shift(const Point2f& shift);
	Point2f top_right_shift(const Point2f& shift);
	Point2f bot_left_shift(const Point2f& shift);
	Point2f bot_right_shift(const Point2f& shift);

private:
	Point2f top_left;
	Point2f top_right;
	Point2f bot_right;
	Point2f bot_left;
};

#endif /* SRC_CORNERS_H_ */
