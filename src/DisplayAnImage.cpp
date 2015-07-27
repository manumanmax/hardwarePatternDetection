/*
 * DisplayAnImage.cpp

 *
 *  Created on: 20 mai 2015
 *      Author: manu
 */

#define SMALL_THRESHOLD 0.001
#define HIGH_THRESHOLD  235 // 212 highLight // 235 easyLowLight
#define MAX_NUMBER_OF_PATTERN 10

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



void value_tester(string sceneName, string patternName, const int numberOfPatterns) {
	vector<Pattern> patterns;
	int baseValueT_init = 100;
	int baseValueT = baseValueT_init;
	int baseValueH = 400;

	Pattern pattern(patternName, baseValueH, baseValueT);
	Scene scene1(sceneName);
	Corners corner;
	Mat final_img = scene1.init_an_image(Pattern(sceneName));

	//for (; baseValueH < 500; baseValueH += 100) {
		while (scene1.patterns.size() < numberOfPatterns && baseValueT < 250) {
			scene1.patterns.clear();
			baseValueT += 1;
			pattern.treshold = baseValueT;
			scene1.matche_scene(pattern);
			scene1.show_matches(pattern, final_img);
			scene1.init_before_search(pattern);

			while (scene1.searchPattern(final_img, corner, pattern, 1))
				;
		}
		std::cout << "base value threshold : " << baseValueT << std::endl;
		std::cout << "base value minHessian: " << baseValueH << std::endl;
		string name = "found with ";
		name += std::to_string(baseValueH);
		name += " and ";
		name += std::to_string(baseValueT);
		//imshow(name, final_img);
		baseValueT = baseValueT_init;
		pattern = Pattern("../Ressources/models/italien.jpg", baseValueH,
				baseValueT);

	//}
	imshow("final image", final_img);
	for (unsigned int i = 0; i < scene1.patterns.size(); i++) {
		scene1.patterns[i].printComponent();
	}

}

/************************************************************************************************************/
// ----------------------------------------------- MAIN --------------------------------------------------- //
int main(int argc, char* argv[]) {
	if (argc > 1) {
		string substring = string(argv[1]);
		string sceneName = "../Ressources/views/" + substring + ".jpg";
		if (argc == 5 && string(argv[4]) == "test") {
			string patternName = "../Ressources/models/" + string(argv[2]) + ".jpg";
			value_tester(sceneName, patternName, atoi(argv[3]));
		} else if (argc > 1) {

			vector<Pattern> patterns;
			//patterns.push_back(Pattern("../Ressources/models/dustBeanBlack.jpg", 1400, 140));
			for (int i = 2; i < argc; i = i + 2) {
				string patternName = "../Ressources/models/" + string(argv[i])
						+ ".jpg";
				patterns.push_back(
						Pattern(patternName, 400, atoi(argv[i + 1])));
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
				//scene1.show_matches(patterns[i], final_img);
				scene1.init_before_search(patterns[i]);

				while (scene1.searchPattern(final_img, corner, patterns[i], 0))
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
