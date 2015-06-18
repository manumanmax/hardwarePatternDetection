/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#include <string>
#define SMALL_THRESHOLD 0.001
#define HIGH_THRESHOLD 300

#include <stdio.h>
#include <iostream>
#include <cv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <highgui.h>

using namespace cv;

void detect(const Mat& img1, const Mat& img2, vector<KeyPoint>& keypoints1,
		vector<KeyPoint>& keypoints2) {
	SiftFeatureDetector detector(1500);
	detector.detect(img1, keypoints1);
	detector.detect(img2, keypoints2);
}

void compute(const Mat& img1, const Mat& img2, vector<KeyPoint>& keypoints1,
		vector<KeyPoint>& keypoints2, Mat& descriptors1, Mat& descriptors2) {
	cv::SiftDescriptorExtractor extractor;
	extractor.compute(img1, keypoints1, descriptors1);
	extractor.compute(img2, keypoints2, descriptors2);
}

std::vector<DMatch> filter(const std::vector<DMatch> matches) {
	vector<DMatch> good_matches;
	for (unsigned int i = 0; i < matches.size(); i++) {
		if (matches[i].distance < HIGH_THRESHOLD)
			good_matches.push_back(matches[i]);
	}
	return good_matches;
}

void draw(const Mat& img1, const Mat& img2, vector<KeyPoint>& keypoints1,
		vector<KeyPoint>& keypoints2, vector<DMatch>& good_matches,
		Mat& img_matches) {
	namedWindow("matches", 1);
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches);
	imshow("matches", img_matches);
}

void detect_corners(std::vector<Point2f>& obj_corners, const Mat& img_object) {
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows);
	obj_corners[3] = cvPoint(0, img_object.rows);
	/*for (unsigned int i = 0; i < obj_corners.size(); i++) {
		std::cout << "object corners [" << i << "] : " << obj_corners[i].x
				<< "," << obj_corners[i].y << std::endl;
	}*/
}

void draw_final_image(Mat& img_matches,
		const std::vector<Point2f>& scene_corners, const Mat& img_object) {
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0),
			scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 0, 255),
			4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0),
			scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 0, 255),
			4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0),
			scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 0, 255),
			4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0),
			scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 0, 255),
			4);

	imshow("final image", img_matches);
}

// annexe functions

void print_matches(std::vector<DMatch> matches){
	for(unsigned int i = 0; i < matches.size(); i++){
		std::cout << "Train idex of match[" << i << "] : " <<matches[i].queryIdx;
	}
}


// ------ MAIN ------ //

int main(int argc, char* argv[]) {

// Initialisati
	Mat img1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_matches;
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	if (img1.empty() || img2.empty()) {
		printf("Can't read one of the images\n");
		return -1;
	}

// detecting keypoints
	detect(img1, img2, keypoints1, keypoints2);
// computing descriptors
	compute(img1, img2, keypoints1, keypoints2, descriptors1, descriptors2);
// matching descriptors
	matcher.match(descriptors1, descriptors2, matches);
// reduce the number of matches
	std::vector<DMatch> good_matches = filter(matches);
// drawing the results
	draw(img1, img2, keypoints1, keypoints2, good_matches, img_matches);

//------- SECOND PART DETECT PATTERNS ---------------//

//Initialisation
	Mat H;
	std::vector<Point2f> obj_corners_unit(4);
	std::vector<Point2f> obj_corners(4);
	std::vector<Point2f> scene_corners(4);
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

//filling object and scene matrix
	for (unsigned int j = 0; j < good_matches.size(); j++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[good_matches[j].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[j].trainIdx].pt);
	}

//Homography
	H = findHomography(obj, scene, CV_RANSAC);

//Detect corners
	detect_corners(obj_corners, img1);

	perspectiveTransform(obj_corners, scene_corners, H);

//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	draw_final_image(img_matches, scene_corners, img1);


	waitKey(0);

}

