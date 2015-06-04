/*
 * Trainer.cpp
 *
 *  Created on: 1 juin 2015
 *      Author: manu
 */
#include "Trainer.h"

Trainer::Trainer() {

}
void Trainer::load(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors,
		std::string kp_desc_file) {
}

void Trainer::save(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors,
		std::string kp_desc_file) {
	fs = cv::FileStorage(kp_desc_file, cv::FileStorage::APPEND);
	std::string name = "_1";
	if (fs.isOpened()) {
		cv::write( fs , name, keypoints );
		fs.release();
	}else{
		std::cout << "failed open the file" << std::endl;
	}
}

void Trainer::train() {

}
