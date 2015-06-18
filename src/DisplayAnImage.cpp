/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#include <string>
#define SMALL_THRESHOLD 0.001
#define HIGH_THRESHOLD 2300
#include <cv.h>
#include <highgui.h>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;

void readme();
std::string describe(DMatch);
void multipleModels(int argc, char** argv);
void multipleScene(int argc, char** argv);
std::vector<std::vector<DMatch>> sortMatches(std::vector<DMatch>);
std::vector<DMatch> selectDMatchByIndex(std::vector<DMatch> good_matches, int index);

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
	} else if (argc != numberOfModels + 4) {
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
	std::vector<std::vector<DMatch>> matches[numberOfModels];
	//FlannBasedMatcher matcher;
	BFMatcher matcher(NORM_L1);
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
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);
	SiftDescriptorExtractor extractor;

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

		// filtering matches for better recognition
		for (unsigned int j = 0; j < matches[i].size(); j++) {
			for (int k = 0; k < descriptors_object[i].rows; k++) {
				if (matches[i][j][k].distance > SMALL_THRESHOLD
						&& matches[i][j][k].distance < HIGH_THRESHOLD) {
					good_matches[i].push_back(matches[i][j][k]);
					//std::cout << discribe(matches[i][j][k]) << std::endl;
				}
			}
		}

		//sortMatches(good_matches[i]);
		good_matches[i] = selectDMatchByIndex(good_matches[i],atoi(argv[4]));
		std::cout << "good matches after fonction : " << std::endl;
		for(unsigned int j = 0; j < good_matches[i].size(); j++){
				describe(good_matches[i][j]);
			}
		for(int j = 0; j < good_matches[j].size(); j++){
			describe(good_matches[i][j]);
		}


		Mat img_matches;
		drawMatches(img_object[i], keypoints_object[i], img_scene,
				keypoints_scene, good_matches[i], img_matches);
		imshow("img", img_matches);


		/*drawMatches(img_object[i], keypoints_object[i], img_scene,
		 keypoints_scene, good_matches[i], img_matches, Scalar::all(-1),
		 Scalar::all(-1), vector<char>(),
		 DrawMatchesFlags::DRAW_OVER_OUTIMG);*/

		//-- Localize the object
		/*for (unsigned int j = 0; j < good_matches[i].size(); j++) {
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
		 //imshow("homography", H[i]);
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
		 Scalar(0, 0, 255), 4);*/
	}
	//-- Show detected matches
	string finish_Name = "Good Matches & Object detection number "
			+ std::to_string(i);
	//imshow(finish_Name, img_matches);

	waitKey(0);
}

std::vector<std::vector<DMatch>> sortMatches(std::vector<DMatch> good_matches) {
	std::vector<std::vector<DMatch>> filtred;

	int max_simultaneus_matches = 0;
	for (unsigned int i = 0; i < good_matches.size(); i++) {
		int current_max_matches = 0;
		std::vector<DMatch> current_class;
		DMatch current_match = good_matches[i];
		for (unsigned int j = 0; j < good_matches.size(); j++) {
			if (j == i)
				continue;

			if (good_matches[j].queryIdx == good_matches[i].queryIdx) {
				current_max_matches++;
				good_matches[j] = DMatch();
			}
		}
		//std::cout << "current_max_matches : "  << current_max_matches << std::endl;
		if (current_max_matches > max_simultaneus_matches) {
			max_simultaneus_matches = current_max_matches;
		}
	}
	std::cout << "max_simultaneus_matches : " << max_simultaneus_matches
			<< std::endl;
	return filtred;
}

std::vector<DMatch> selectDMatchByIndex(std::vector<DMatch> good_matches, int index) {
	std::set<DMatch> good_matches_set;
	index = good_matches[index].queryIdx;
	//unsigned int distance = good_matches[index].distance;
	std::cout << "Analyse of match :" << std::endl;
	describe(good_matches[index]);
	for (unsigned int j = 0; j < good_matches.size(); j++) {
		if (good_matches[j].queryIdx == index){//&& good_matches[j].distance == distance) {
			good_matches_set.insert(good_matches_set.end(),good_matches[j]);
		}
	}
	good_matches.clear();
	std::copy(good_matches_set.begin(), good_matches_set.end(), std::back_inserter(good_matches));
	std::cout << "good matches after copy : " << std::endl;

	for(unsigned int i = 0; i < good_matches.size(); i++){
		describe(good_matches[i]);
	}
	return good_matches;
}

std::string describe(DMatch match) {
	std::string string = "match found:\n";
	std::cout << "Distance : " << match.distance << "\n";
	std::cout << "Image index : " << match.imgIdx << "\n";
	std::cout << "Query index : " << match.queryIdx << "\n";
	std::cout << "Train index : " << match.trainIdx << "\n";
	return string;
}

/** @function readme */
void readme() {
	std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl;
}
