/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#define SMALL_THRESHOLD 0.001
#define HIGH_THRESHOLD  235 // 212 highLight // 235 easyLowLight
#define MAX_NUMBER_OF_PATTERN 10
#define MAX_NUMBER_OF_REPEAT 10

#include <string>
#include <stdio.h>
#include <iostream>
#include <cv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <highgui.h>

// local includes
#include "Scene.h"

using namespace cv;
/*************************************************Values Tester**********************************************/

bool stop_condition(const int maxNumberFound, const int numberFound,
		bool& increasing, int& repeats) {
	if(repeats == MAX_NUMBER_OF_REPEAT) return true;
	repeats++;
	if (increasing) {
		std::cout << "increasing : max : " << maxNumberFound << ", found : "
				<< numberFound << std::endl;
		if (numberFound <= maxNumberFound) {
			increasing = false;
		} else {
			return false;
		}
	} else {
		std::cout << "not increasing : max : " << maxNumberFound << ", found : "
				<< numberFound << std::endl;

		if (maxNumberFound < numberFound) {
			increasing = true;
		} else if (maxNumberFound == numberFound) {
			increasing = false;
		} else if (maxNumberFound > numberFound) {
			return true;
		}
		return false;
	}
}

void value_tester(string sceneName) {
	vector<Pattern> patterns;
	int baseValueT_init = 150;
	int baseValueT = baseValueT_init;
	int baseValueH = 100;
	int maxNumberFound = 0;
	int repeats = 0;
	bool increasing = false;

	Pattern pattern("../Ressources/models/italien.jpg", baseValueH, baseValueT);
	Scene scene1(sceneName);
	Corners corner;
	Mat final_img = scene1.init_an_image(Pattern(sceneName));

	for (; baseValueH < 1600; baseValueH += 100) {
		maxNumberFound = 0;
		while (!stop_condition(maxNumberFound, scene1.patterns.size(),
				increasing) && baseValueT < 250) {

			scene1.patterns.clear();
			baseValueT += 1;
			pattern.treshold = baseValueT;
			scene1.matche_scene(pattern);
			scene1.show_matches(pattern, final_img);
			scene1.init_before_search(pattern);

			while (scene1.searchPattern(final_img, corner, pattern, 1))
				;
			if (maxNumberFound < scene1.patterns.size())
				maxNumberFound = scene1.patterns.size();
		}
		std::cout << "base value threshold : " << baseValueT << std::endl;
		std::cout << "base value minHessian: " << baseValueH << std::endl;
		string name = "found with ";
		name += std::to_string(baseValueH);
		name += " and ";
		name += std::to_string(baseValueT);
		imshow(name, final_img);
		baseValueT = baseValueT_init;
		pattern = Pattern("../Ressources/models/italien.jpg", baseValueH,
				baseValueT);
		for (unsigned int i = 0; i < scene1.patterns.size(); i++) {
			scene1.patterns[i].printComponent();
		}
	}

}

/************************************************************************************************************/
// ----------------------------------------------- MAIN --------------------------------------------------- //
int main(int argc, char* argv[]) {
	if (argc > 1) {
		string substring = string(argv[1]);
		string sceneName = "../Ressources/views/" + substring + ".jpg";
		if (argc == 3 && string(argv[2]) == "test") {
			value_tester(sceneName);
		} else if (argc > 1) {

			vector<Pattern> patterns;
			//patterns.push_back(Pattern("../Ressources/models/dustBeanBlack.jpg", 1400, 140));
			for (int i = 2; i < argc; i = i + 2) {
				string patternName = "../Ressources/models/" + string(argv[i])
						+ ".jpg";
				patterns.push_back(
						Pattern(patternName, 1500, atoi(argv[i + 1])));
				//Pattern("../Ressources/models/MIU.jpg", 1500, 235)
			}
			//patterns.push_back(Pattern("../Ressources/models/fullRacClear.jpg",400, 150));
			//patterns.push_back(Pattern("../Ressources/models/italien.jpg", 400, 150));

			Scene scene1(sceneName);
			Corners corner;
			Mat final_img = scene1.init_an_image(Pattern(sceneName));

			for (unsigned int i = 0; i < patterns.size(); i++) {
				//std::cout << "detection of pattern " << i << std::endl;

				scene1.matche_scene(patterns[i]);
				scene1.show_matches(patterns[i], final_img);
				scene1.init_before_search(patterns[i]);

				while (scene1.searchPattern(final_img, corner, patterns[i], 1))
					;
			}
			imshow("final image", final_img);
			for (unsigned int i = 0; i < scene1.patterns.size(); i++) {
				scene1.patterns[i].printComponent();
			}
		}

		waitKey(0);
	}
}
