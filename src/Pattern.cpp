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

string Pattern::getFileName(const string& s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return (s.substr(i + 1, s.length() - i));
	}

	return ("");
}

Pattern::Pattern(string location, const int minHessian) {

	img = imread(location, CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		std::cout << "can't read pattern image : " << location << std::endl;
	} else {
		cv::equalizeHist(img, img);
		SiftFeatureDetector detector(minHessian);
		detector.detect(img, keypoints);
		cv::SiftDescriptorExtractor extractor;
		extractor.compute(img, keypoints, descriptors);
		name = getFileName(location);
	}
}


void Pattern::printPattern(){
	std::cout << "Pattern : " << std::endl << name << std::endl;
}
