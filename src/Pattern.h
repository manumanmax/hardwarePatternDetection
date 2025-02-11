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
//#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <highgui.h>

using namespace cv;

class Pattern {
public:
	Pattern();
	Pattern(string location, const int minHessian, const double treshold);
	Pattern(string location);
	string getFileName(const string& location);
	void printPattern();
	virtual ~Pattern();
	Mat img;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	string name;
	double treshold;
private:
	unsigned int minHessian;



};

#endif /* SRC_PATTERN_H_ */
