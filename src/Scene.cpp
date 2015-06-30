/*
 * Scene.cpp
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#include "Scene.h"

Scene::Scene() {
	// TODO Auto-generated constructor stub
}


bool Scene::searchPattern(Mat& img_matches, Corners& corners, const Pattern& pattern) {
	if (!patternInitialised){
		std::cout << "pattern not initialised" << std::endl;
		return false;
	}
	if (good_matches.size() <= 15) {
		std::cout << "too few matches" << std::endl;
		return false;
	}
	//draw(img1, img2, keypoints1, keypoints2, good_matches, img_matches);
	corners = find_object(img_matches, pattern);
	int numberOfPointRemoved = removePointsOfObjectFound(corners);
	if (numberOfPointRemoved > 15){
		add_component(pattern,corners);
		return true;
	}
	return false;
}



Corners Scene::find_object(Mat& img_matches,const Pattern& pattern) {

	//Homography
	//print_points2f(obj);
	H = findHomography(obj, scene, CV_RANSAC);

	//Detect corners
	detect_corners(pattern.img);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	Corners corners = draw_final_image(img_matches,pattern);

	return corners;
}



void Scene::init_before_search(Pattern& pattern, const int treshold) {
	matche_scene(pattern, treshold);
	pattern.printPattern();
//Initialisation
	patternInitialised = true;
//filling object and scene matrix
	for (unsigned int j = 0; j < good_matches.size(); j++) {
//-- Get the keypoints from the good matches
		obj.push_back(pattern.keypoints[good_matches[j].queryIdx].pt);
		scene.push_back(keypoints[good_matches[j].trainIdx].pt);
	}
}

// ----------------------------- ANNEXES FUNCTIONS -------------------------------------
/***************************************************************************************/

Mat Scene::init_an_image(const Pattern& pattern) {
	Mat img_matches;
	Size size(pattern.img.cols + img.cols, img.rows);
	img_matches.create(size,
			CV_MAKETYPE(pattern.img.depth(), pattern.img.channels()));
	cv::Rect roi(cv::Point(img_matches.size().width - img.cols, 0), img.size());

	img.copyTo(img_matches(roi));

	return img_matches;
}



void Scene::matche_scene(const Pattern& pattern, const int treshold) {
	vector<vector<DMatch>> matches;

	matcher.radiusMatch(pattern.descriptors, descriptors, matches, treshold);
	fulfil(good_matches, matches);
	//print_matches(good_matches);
}



/* TODO: make a return value if less than a certain number of points are deleted */
int Scene::removePointsOfObjectFound(Corners corners) {
	// we have to remove both the scene and object indices because they are pushed back together
	int index = 0;
	int numberOfPointsRemoved = 0;
	auto it_obj = obj.begin();
	for (auto it_scene = scene.begin(); it_scene != scene.end();) {
		if (corners.is_in(*it_scene)) {
			numberOfPointsRemoved++;
			it_scene = scene.erase(it_scene);
			it_obj = obj.erase(it_obj);
			good_matches.erase(good_matches.begin() + index);
		} else {
			it_scene++;
			it_obj++;
			index++;
		}
	}
	return numberOfPointsRemoved;
}


void Scene::detect_corners(const Mat& img_object) {
	obj_corners.push_back(cvPoint(0, 0));
	obj_corners.push_back(cvPoint(img_object.cols, 0));
	obj_corners.push_back(cvPoint(img_object.cols, img_object.rows));
	obj_corners.push_back(cvPoint(0, img_object.rows));
}


Corners Scene::draw_final_image(Mat& img_matches, const Pattern& pattern) {
	Corners corners(scene_corners[0],scene_corners[1],scene_corners[2],scene_corners[3]);
	Point2f right_sift = Point2f(pattern.img.cols, 0);
	line(img_matches, corners.top_left_shift(right_sift),
			corners.top_right_shift(right_sift), Scalar(0, 0, 255), 4);
	line(img_matches, corners.top_right_shift(right_sift),
			corners.bot_right_shift(right_sift), Scalar(0, 0, 255), 4);
	line(img_matches, corners.bot_right_shift(right_sift),
			corners.bot_left_shift(right_sift), Scalar(0, 0, 255), 4);
	line(img_matches, corners.bot_left_shift(right_sift),
			corners.top_left_shift(right_sift), Scalar(0, 0, 255), 4);
	imshow("final image ", img_matches);
	return corners;
}



void Scene::print_matches(std::vector<DMatch> matches) {
	for (unsigned int i = 0; i < matches.size(); i++) {
		std::cout << "Train idex of match[" << i << "] : "
				<< matches[i].queryIdx << std::endl;
	}
}


void Scene::fulfil(vector<DMatch>& matches,
		const vector<vector<DMatch>>& matches_knnVector) {
	for (unsigned int i = 0; i < matches_knnVector.size(); i++) {
		for (unsigned int j = 0; j < matches_knnVector[i].size(); j++) {
			matches.push_back(matches_knnVector[i][j]);
		}
	}
}

void Scene::show_matches(const Pattern& pattern, Mat& img_matches){
	drawMatches(pattern.img, pattern.keypoints, img, keypoints, good_matches, img_matches);
}



// -------------------------------- CONSTRUCTOR - DESTRUCTOR --------------------------------------



Scene::Scene(string location) {
	img = imread(location, CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		std::cout << "can't read scene image : " << location << std::endl;
	} else {
		patterns = std::vector<Composant>();
		patternInitialised = false;
		cv::equalizeHist(img, img);
		SiftFeatureDetector detector(10000);
		detector.detect(img, keypoints);
		cv::SiftDescriptorExtractor extractor;
		extractor.compute(img, keypoints, descriptors);
	}
}

void Scene::add_component(const Pattern& pattern,const Corners& corners){
	patterns.push_back(Composant(pattern.name,corners));
}

Scene::~Scene() {
	//obj_corners_unit.clear();
	//obj_corners.clear();
	//scene_corners.clear();
	//obj.clear();
	//scene.clear();
}

