/*
 * Trainer.h
 *
 *  Created on: 1 juin 2015
 *      Author: manu
 */

#ifndef INCLUDES_TRAINER_H_
#define INCLUDES_TRAINER_H_

#include "opencv2/core/core.hpp"
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <string>

class Trainer{
private:
	cv::FileStorage fs;

public:
	Trainer();
	void load(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors, std::string);
	void save(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors, std::string);
	void train();
};


#endif /* INCLUDES_TRAINER_H_ */
