/*
 * Pattern.cpp
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#include "Pattern.h"

Pattern::Pattern() {
	// TODO Auto-generated constructor stub

}

Pattern::~Pattern() {
	// TODO Auto-generated destructor stub
}

Pattern::Pattern(string location) {

	img = imread(location, CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		printf("Can't read one of the images\n");
	} else {
		cv::equalizeHist(img, img);
		SiftFeatureDetector detector(1000000000);
		detector.detect(img, keypoints);
		cv::SiftDescriptorExtractor extractor;
		extractor.compute(img, keypoints, descriptors);
	}
}
