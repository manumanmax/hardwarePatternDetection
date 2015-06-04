/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#include "Trainer.h"
#include <cv.h>
#include <highgui.h>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;
int minHessian = 1500;
SurfFeatureDetector detector(minHessian);
SurfDescriptorExtractor extractor;


void readme();
void multipleModels(int argc, char** argv);

std::vector<cv::KeyPoint> extractKeypoints(cv::Mat matrix){

	std::vector<KeyPoint> keypoints;

	//-- Step 2: Calculate descriptors (feature vectors)
	detector.detect(matrix, keypoints);

	return keypoints;
}

cv::Mat extractDescriptor(cv::Mat matrix, std::vector<KeyPoint>& keypoints){
	cv::Mat descriptors_object;
	extractor.compute(matrix, keypoints, descriptors_object);
	return descriptors_object;
}

int main(int argc, char** argv) {
	//multipleModels(argc, argv);

	Trainer trainer;
	if (argc < 2) {
		readme();
	} else {
		Mat img_model = imread(argv[1], CV_LOAD_IMAGE_COLOR);
		if(img_model.data){
			imshow("model", img_model);
			//std::cout << extractKeypoints(img_model).size() << std::endl;
			string s = "../Ressources/trainningSets/item/_one.yml";
			trainer.save(extractKeypoints(img_model),img_model,s);
		}else{
			return 0;
		}
	}
	waitKey(0);
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
	Mat img_scene = imread(argv[2], CV_LOAD_IMAGE_COLOR);
	//imshow("scene to analyse", img_scene);
	int i = numberOfModels;
	while (i-- > 0) {
		img_object[i] = imread(argv[3 + i], CV_LOAD_IMAGE_COLOR);
		if (!img_object[i].data) {
			std::cout << i << " --(!) Error reading images model " << std::endl;
			return;
		}
	}
	if (!img_scene.data) {
		std::cout << " --(!) Error reading images source " << std::endl;
		return;
	}

	//find the largest model image for drawmatches:
	i = numberOfModels;
	int width = 0;
	while (i-- > 0) {
		if (img_object[i].size().width > width)
			width = img_object[i].size().width;
	}
	Size size(width + img_scene.size().width, img_scene.size().height);
	std::cout << "maximum object size : " << width << std::endl;
	// initialization
	double dist;
	double max_dist[numberOfModels];
	double min_dist[numberOfModels];
	Mat H[numberOfModels];
	std::vector<Point2f> obj_corners_unit(4);
	std::vector<std::vector<Point2f>> obj_corners(numberOfModels);
	for (int i = 0; i < numberOfModels; i++) {
		obj_corners[i] = obj_corners_unit;
	}
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
	img_matches.create(size, CV_MAKETYPE(img_scene.depth(), 3));

	cv::Rect roi(
			cv::Point(img_matches.size().width - img_scene.size().width, 0),
			img_scene.size());
	std::cout << img_matches.channels() << std::endl;
	std::cout << img_scene.channels() << std::endl;

	img_scene.copyTo(img_matches(roi));

	std::cout << "size of the output image : " << img_matches.size()
			<< std::endl;

	//-- Step 1: Detect the keypoints using SURF Detector
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
			if (matches[i][j].distance < 5 * min_dist[i]) {
				good_matches[i].push_back(matches[i][j]);
			}
		}

		std::cout << "size of the scene image : " << img_scene.size()
				<< std::endl;
		std::cout << "size of the model image : " << img_object[i].size()
				<< std::endl;

		/*drawMatches(img_object[i], keypoints_object[i], img_scene,
		 keypoints_scene, good_matches[i], img_matches, Scalar::all(-1),
		 Scalar::all(-1), vector<char>(),
		 DrawMatchesFlags::DRAW_OVER_OUTIMG);*/

		//-- Localize the object
		for (unsigned int j = 0; j < good_matches[i].size(); j++) {
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

		obj_corners[i][0] = cvPoint(0, 0);
		obj_corners[i][1] = cvPoint(img_object[i].cols, 0);
		obj_corners[i][2] = cvPoint(img_object[i].cols, img_object[i].rows);
		obj_corners[i][3] = cvPoint(0, img_object[i].rows);

		perspectiveTransform(obj_corners[i], scene_corners, H[i]);

		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		line(img_matches, scene_corners[0] + Point2f(img_object[i].cols, 0),
				scene_corners[1] + Point2f(img_object[i].cols, 0),
				Scalar(0, 0, 255), 4);
		line(img_matches, scene_corners[1] + Point2f(img_object[i].cols, 0),
				scene_corners[2] + Point2f(img_object[i].cols, 0),
				Scalar(0, 0, 255), 4);
		line(img_matches, scene_corners[2] + Point2f(img_object[i].cols, 0),
				scene_corners[3] + Point2f(img_object[i].cols, 0),
				Scalar(0, 0, 255), 4);
		line(img_matches, scene_corners[3] + Point2f(img_object[i].cols, 0),
				scene_corners[0] + Point2f(img_object[i].cols, 0),
				Scalar(0, 0, 255), 4);
	}
	//-- Show detected matches
	string finish_Name = "Good Matches & Object detection number "
			+ std::to_string(i);
	imshow(finish_Name, img_matches);

	waitKey(0);
}

/** @function readme */
void readme() {
	std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl;
}
