/*
 * Pattern.h
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#ifndef SRC_PATTERN_H_
#define SRC_PATTERN_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <cv.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;

class Pattern {
public:
	Pattern();
	virtual ~Pattern();
private:
	Mat img;
	vector<KeyPoint> keypoints;
	Mat descriptors;
};

#endif /* SRC_PATTERN_H_ */
