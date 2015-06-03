/*
 * Trainer.cpp
 *
 *  Created on: 1 juin 2015
 *      Author: manu
 */
#include "Trainer.h"
#include <opencv2/features2d/features2d.hpp>

Trainer::Trainer() {

}
void Trainer::load(std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors,
		std::string kp_desc_file) {

}

void Trainer::save(std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors,
		std::string kp_desc_file) {
	fs = cv::FileStorage(kp_desc_file, cv::FileStorage::WRITE);

	fs << "descriptors" << descriptors;
	fs << "keypoints" << keypoints;
}

void Trainer::train() {

}
