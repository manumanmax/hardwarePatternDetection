/*
 * Scene.h
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_
#define MAX_NUMBER_OF_PATTERN 10

#include <vector>
#include "Composant.h"
#include "Pattern.h"

#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <highgui.h>
using namespace std;

class Scene {
public:
	Scene();
	Scene(string location);
	bool searchPattern(Mat& img_matches, Corners& corners,
			const Pattern& pattern);
	Mat init_an_image(Pattern pattern);
	void matche_scene(Pattern pattern);
	void init_before_search(Pattern pattern);
	virtual ~Scene();
	void Scan();

private:
	void fulfil(vector<DMatch>& matches,
			const vector<vector<DMatch>>& matches_knnVector);
	int removePointsOfObjectFound(Corners corners);
	Corners find_object(Mat& img_matches, const Pattern& pattern);
	void detect_corners(const Mat& img_object);
	Corners draw_final_image(Mat& img_matches, const Pattern& pattern);
	void print_matches(std::vector<DMatch> matches);
	bool patternInitialised;

	Mat img;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	FlannBasedMatcher matcher;
	vector<DMatch> good_matches;
	vector<Composant> patterns;
	Mat H;
	std::vector<Point2f> obj_corners_unit;
	std::vector<Point2f> obj_corners;
	std::vector<Point2f> scene_corners;
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

};

#endif /* SRC_SCENE_H_ */
