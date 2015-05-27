/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#include <string>

#include <cv.h>
#include <highgui.h>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;

void readme();
void multipleModels(int argc, char** argv);
void multipleScene(int argc, char** argv);

int main(int argc, char** argv) {
	multipleModels(argc, argv);

	return 0;
}

void multipleModels(int argc, char** argv) {
	Mat image;

	int numberOfModels = atoi(argv[1]);
	if (argc < 4) {
		readme();
		return;
	} else if (argc != numberOfModels + 3) {
		readme();
		return;
	}

	// init phase
	Mat img_object[numberOfModels];
	Mat img_scene = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);

	int i = numberOfModels;
	while (i-- > 0) {
		img_object[i] = imread(argv[3 + i], CV_LOAD_IMAGE_GRAYSCALE);
		if (!img_object[i].data) {
			std::cout << i << " --(!) Error reading images model " << std::endl;
			return;
		}
	}
	if (!img_scene.data) {
		std::cout << " --(!) Error reading images source " << std::endl;
		return;
	}
	// initialization
	double dist;
	double max_dist[numberOfModels];
	double min_dist[numberOfModels];
	Mat H[numberOfModels];
	std::vector<Point2f> obj_corners(4);
	std::vector<Point2f> scene_corners(4);
	std::vector<Point2f> obj[numberOfModels];
	std::vector<Point2f> scene[numberOfModels];
	std::vector<KeyPoint> keypoints_object[numberOfModels], keypoints_scene;
	Mat descriptors_object[numberOfModels], descriptors_scene;
	std::vector<DMatch> matches[numberOfModels];
	FlannBasedMatcher matcher;
	//BFMatcher matcher(NORM_L1);
	std::vector<DMatch> good_matches[numberOfModels];
	Mat img_matches;

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 1500;

	SurfFeatureDetector detector(minHessian);
	SurfDescriptorExtractor extractor;

	//-- Step 2: Calculate descriptors (feature vectors)
	detector.detect(img_scene, keypoints_scene);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	// main treatement loop
	for (int i = 0; i < numberOfModels; i++) {

		detector.detect(img_object[i], keypoints_object[i]);
		//-- Step 2: Calculate descriptors (feature vectors)
		extractor.compute(img_object[i], keypoints_object[i],
				descriptors_object[i]);
		//-- Step 3: Matching descriptor vectors using FLANN matcher
		matcher.match(descriptors_object[i], descriptors_scene, matches[i]);
		//-- Quick calculation of max and min distances between keypoints
		max_dist[i] = 0.0;
		min_dist[i] = 100.0;
		for (int j = 0; j < descriptors_object[i].rows; j++) {
			dist = matches[i][j].distance;

			if (dist < min_dist[i])
				min_dist[i] = dist;
			if (dist > max_dist[i])
				max_dist[i] = dist;
		}

		printf("-- Max dist[%d] : %f \n", i, max_dist[i]);
		printf("-- Min dist[%d] : %f \n-------------------------\n", i,
				min_dist[i]);

		//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
		for (int j = 0; j < descriptors_object[i].rows; j++) {
			if (matches[i][j].distance < 2.3 * min_dist[i]) {
				good_matches[i].push_back(matches[i][j]);
			}
		}

		drawMatches(img_object[i], keypoints_object[i], img_scene,
				keypoints_scene, good_matches[i], img_matches, Scalar::all(-1),
				Scalar::all(-1), vector<char>(),
				DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//-- Localize the object

		for (int j = 0; j < good_matches[i].size(); j++) {
			//-- Get the keypoints from the good matches
			obj[i].push_back(
					keypoints_object[i][good_matches[i][j].queryIdx].pt);
			scene[i].push_back(keypoints_scene[good_matches[i][j].trainIdx].pt);
		}

		std::cout << "size of obj_list " << obj[i].size() << std::endl;
		std::cout << "size of scene_list " << scene[i].size() << std::endl;
		if (obj[i].empty())
			continue;

		//homography
		H[i] = findHomography(obj[i], scene[i], CV_RANSAC);
		//std::vector<Point2f> obj_corners(4);
		//-- Get the corners from the image_1 ( the object to be "detected" )

		obj_corners[0] = cvPoint(0, 0);
		obj_corners[1] = cvPoint(img_object[i].cols, 0);
		obj_corners[2] = cvPoint(img_object[i].cols, img_object[i].rows);
		obj_corners[3] = cvPoint(0, img_object[i].rows);


		perspectiveTransform(obj_corners, scene_corners, H[i]);

		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		line(img_matches, scene_corners[0] + Point2f(img_object[i].cols, 0),
				scene_corners[1] + Point2f(img_object[i].cols, 0),
				Scalar(255, 255, 255), 4);
		line(img_matches, scene_corners[1] + Point2f(img_object[i].cols, 0),
				scene_corners[2] + Point2f(img_object[i].cols, 0),
				Scalar(255, 255, 255), 4);
		line(img_matches, scene_corners[2] + Point2f(img_object[i].cols, 0),
				scene_corners[3] + Point2f(img_object[i].cols, 0),
				Scalar(255, 255, 255), 4);
		line(img_matches, scene_corners[3] + Point2f(img_object[i].cols, 0),
				scene_corners[0] + Point2f(img_object[i].cols, 0),
				Scalar(255, 255, 255), 4);
	}
	//-- Show detected matches
	string finish_Name = "Good Matches & Object detection number "; //+ std::to_string(i);
	imshow(finish_Name, img_matches);

	waitKey(0);
}

void multipleScene(int argc, char** argv) {
	Mat image;

	if (argc != 4) {
		readme();
		return;
	}

	Mat img_object = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_scene = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_scene2 = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);

	if (!img_object.data || !img_scene.data || !img_scene2.data) {
		std::cout << " --(!) Error reading images " << std::endl;
		return;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);
	std::vector<KeyPoint> keypoints_object, keypoints_scene, keypoints_scene2;

	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);
	detector.detect(img_scene, keypoints_scene2);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene, descriptors_scene2;

	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);
	extractor.compute(img_scene2, keypoints_scene2, descriptors_scene2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	//BFMatcher matcher(NORM_L1);
	std::vector<DMatch> matches;
	std::vector<DMatch> matches2;
	matcher.match(descriptors_object, descriptors_scene, matches);
	matcher.match(descriptors_object, descriptors_scene2, matches2);

	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n-------------------------\n", min_dist);

	double max_dist2 = 0;
	double min_dist2 = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++) {
		double dist = matches2[i].distance;
		if (dist < min_dist2)
			min_dist2 = dist;
		if (dist > max_dist2)
			max_dist2 = dist;
	}
	printf("-- Max dist : %f \n", max_dist2);
	printf("-- Min dist : %f \n", min_dist2);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector<DMatch> good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance < 1.5 * min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

	std::vector<DMatch> good_matches2;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches2[i].distance < 1.5 * min_dist2) {
			good_matches2.push_back(matches2[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	Mat img_matches2;
	drawMatches(img_object, keypoints_object, img_scene2, keypoints_scene2,
			good_matches2, img_matches2, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj2;
	std::vector<Point2f> scene2;

	for (int i = 0; i < good_matches.size(); i++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	for (int i = 0; i < good_matches2.size(); i++) {
		//-- Get the keypoints from the good matches
		obj2.push_back(keypoints_object[good_matches2[i].queryIdx].pt);
		scene2.push_back(keypoints_scene2[good_matches2[i].trainIdx].pt);
		std::cout << scene << std::endl;
	}

	Mat H2 = findHomography(obj2, scene2, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows);
	obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0),
			scene_corners[1] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0),
			scene_corners[2] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0),
			scene_corners[3] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0),
			scene_corners[0] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);

	// -----
	//-- Get the corners from the image_2 ( the object to be "detected" )
	std::vector<Point2f> obj_corners2(4);
	obj_corners2[0] = cvPoint(0, 0);
	obj_corners2[1] = cvPoint(img_object.cols, 0);
	obj_corners2[2] = cvPoint(img_object.cols, img_object.rows);
	obj_corners2[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners2(4);

	perspectiveTransform(obj_corners2, scene_corners2, H2);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches2, scene_corners2[0] + Point2f(img_object.cols, 0),
			scene_corners2[1] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches2, scene_corners2[1] + Point2f(img_object.cols, 0),
			scene_corners2[2] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches2, scene_corners2[2] + Point2f(img_object.cols, 0),
			scene_corners2[3] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);
	line(img_matches2, scene_corners2[3] + Point2f(img_object.cols, 0),
			scene_corners2[0] + Point2f(img_object.cols, 0),
			Scalar(255, 255, 255), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	imshow("withCookiesMatches", img_matches2);

	waitKey(0);
}

/** @function readme */
void readme() {
	std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl;
}
