/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#include <string>
#define SMALL_THRESHOLD 0.001
#define HIGH_THRESHOLD 250

#include <stdio.h>
#include <iostream>
#include <cv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <highgui.h>

using namespace cv;

class Corners {
public:
	Point2f top_left;
	Point2f top_right;
	Point2f bot_right;
	Point2f bot_left;
};

void detect(const Mat& img1, const Mat& img2, vector<KeyPoint>& keypoints1,
		vector<KeyPoint>& keypoints2) {
	SiftFeatureDetector detector(4000);
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
	//namedWindow("matches", 1);
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches);
	//imshow("matches", img_matches);
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

Corners draw_final_image(Mat& img_matches,
		const std::vector<Point2f>& scene_corners, const Mat& img_object) {
	Corners corners;
	Point2f right_sift = Point2f(img_object.cols, 0);
	corners.top_left = scene_corners[0];
	corners.top_right = scene_corners[1];
	corners.bot_right = scene_corners[2];
	corners.bot_left = scene_corners[3];

	line(img_matches, corners.top_left + right_sift,
			corners.top_right + right_sift, Scalar(0, 0, 255), 4);
	line(img_matches, corners.top_right + right_sift,
			corners.bot_right + right_sift, Scalar(0, 0, 255), 4);
	line(img_matches, corners.bot_right + right_sift,
			corners.bot_left + right_sift, Scalar(0, 0, 255), 4);
	line(img_matches, corners.bot_left + right_sift,
			corners.top_left + right_sift, Scalar(0, 0, 255), 4);
	imshow("final image ", img_matches);
	return corners;
}

/******************************************************************************************************/
//------------------------------------------------ ANNEXE FUNCTIONS ------------------------------------
void fulfil(vector<DMatch>& matches,
		const vector<vector<DMatch>>& matches_knnVector) {
	for (unsigned int i = 0; i < matches_knnVector.size(); i++) {
		for (unsigned int j = 0; j < matches_knnVector[i].size(); j++) {
			matches.push_back(matches_knnVector[i][j]);
		}
	}
}
void print_matches(std::vector<DMatch> matches) {
	for (unsigned int i = 0; i < matches.size(); i++) {
		std::cout << "Train idex of match[" << i << "] : "
				<< matches[i].queryIdx << std::endl;
	}
}

void print_corners(Corners corners) {
	std::cout << "Corners : " << std::endl << "(" << corners.top_left.x << ","
			<< corners.top_left.y << ")" << " - (" << corners.top_right.x << ","
			<< corners.top_right.y << ")" << std::endl << "("
			<< corners.bot_left.x << "," << corners.bot_left.y << ")" << " - ("
			<< corners.bot_right.x << "," << corners.bot_right.y << ")"
			<< std::endl;
}

void print_points2f(std::vector<Point2f> points) {
	std::cout << "Points 2f :" << std::endl;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (!(i % 3))
			std::cout << i << " : [ " << points[i].x << " , " << points[i].y
					<< " ]" << std::endl;
		else
			std::cout << i << " : [ " << points[i].x << " , " << points[i].y
					<< " ]        ";
	}
}

void print_knnmatches(vector<vector<DMatch>> matches) {
	for (unsigned int i = 0; i < matches.size(); i++) {
		for (unsigned int j = 0; j < matches.size(); j++) {
			std::cout << "Train idex of match[" << i << "][" << j << "] : "
					<< matches[i][j].queryIdx;
		}
	}
}

bool is_in(Corners corners, Point2d point) {
	if (point.x < corners.top_left.x || point.x > corners.top_right.x)
		return false;
	if (point.y < corners.top_left.y || point.y > corners.bot_right.y)
		return false;

	return true;
}

void removePointsOfObjectFound(const Corners corners, vector<Point2f>& scene,
		vector<Point2f>& obj, vector<DMatch> & good_matches) {
	// we have to remove both the scene and object indices because they are pushed back together
	int index = 0;
	auto it_obj = obj.begin();
	for (auto it_scene = scene.begin(); it_scene != scene.end();) {
		if (is_in(corners, *it_scene)) {
			it_scene = scene.erase(it_scene);
			it_obj = obj.erase(it_obj);
			good_matches.erase(good_matches.begin() + index);
		} else {
			it_scene++;
			it_obj++;
			index++;
		}
	}

}

Corners find_object(Mat& H, std::vector<Point2f> obj,
		const std::vector<Point2f>& scene, std::vector<Point2f>& obj_corners,
		const Mat& img1, std::vector<Point2f> scene_corners, Mat& img_matches) {
	//Homography
	print_points2f(obj);
	H = findHomography(obj, scene, CV_RANSAC);

	//Detect corners
	detect_corners(obj_corners, img1);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	Corners corners = draw_final_image(img_matches, scene_corners, img1);

	return corners;
}

/************************************************************************************************************/
// ----------------------------------------------- MAIN --------------------------------------------------- //
int main(int argc, char* argv[]) {

// Initialisati
	Mat img1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	Mat img2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);
	Mat img_matches;
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	FlannBasedMatcher matcher;
	vector<vector<DMatch>> matches_knnVector;
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
	matcher.knnMatch(descriptors1, descriptors2, matches_knnVector, 5);
	//print_knnmatches(matches_knnVector);
	fulfil(matches, matches_knnVector);
// reduce the number of matches
	std::vector<DMatch> good_matches = filter(matches);
// drawing the results
	//draw(img1, img2, keypoints1, keypoints2, good_matches, img_matches);

//------- SECOND PART - DETECT PATTERNS ---------------//

//Initialisation
	Mat H;
	std::vector<Point2f> obj_corners_unit(4);
	std::vector<Point2f> obj_corners(4);
	std::vector<Point2f> scene_corners(4);
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	Size size(img1.cols + img2.cols, img2.rows);
	img_matches.create(size, CV_MAKETYPE(img1.depth(), 3));
	cv::Rect roi(cv::Point(img_matches.size().width - img2.cols, 0),
			img2.size());
	std::cout << img_matches.channels() << std::endl;
	std::cout << img2.channels() << std::endl;

	img2.copyTo(img_matches(roi));

//filling object and scene matrix
	for (unsigned int j = 0; j < good_matches.size(); j++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[good_matches[j].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[j].trainIdx].pt);
	}

//find an object on the scene

	Corners corners = find_object(H, obj, scene, obj_corners, img1,
			scene_corners, img_matches);

	removePointsOfObjectFound(corners, scene, obj, good_matches);

	while (good_matches.size() > 20) {
		//draw(img1, img2, keypoints1, keypoints2, good_matches, img_matches);
		corners = find_object(H, obj, scene, obj_corners, img1, scene_corners,
				img_matches);
		removePointsOfObjectFound(corners, scene, obj, good_matches);
	}

	waitKey(0);
}
