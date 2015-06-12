/*
 * Trainer.cpp
 *
 *  Created on: 1 juin 2015
 *      Author: manu
 */
#include "Trainer.h"

Trainer::Trainer() {
	detector = SurfFeatureDetector(minHessian);
}
void Trainer::load(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors,
		fs::path& path) {
}

void Trainer::save(std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors,
		fs::path& path) {
	fs = cv::FileStorage(path.string(), cv::FileStorage::APPEND);
	std::string name = "_1";
	if (fs.isOpened()) {
		cv::write( fs , name, keypoints );
		fs.release();
	}else{
		std::cout << "failed open the file" << std::endl;
	}
}

void Trainer::saveStoreDir(StoreDirectory& sd){

}

std::vector<cv::KeyPoint> Trainer::extractKeypoints(cv::Mat matrix){

	std::vector<cv::KeyPoint> keypoints;

	//-- Step 2: Calculate descriptors (feature vectors)
	detector.detect(matrix, keypoints);

	return keypoints;
}

cv::Mat Trainer::extractDescriptor(cv::Mat matrix, std::vector<cv::KeyPoint>& keypoints){
	cv::Mat descriptors_object;
	extractor.compute(matrix, keypoints, descriptors_object);
	return descriptors_object;
}

Trainer::~Trainer() {

}

