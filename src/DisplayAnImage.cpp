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
#include "Corners.h"
#include "Scene.h"
#include "Pattern.h"

using namespace cv;

/************************************************************************************************************/
// ----------------------------------------------- MAIN --------------------------------------------------- //
int main(int argc, char* argv[]) {

	vector<Pattern> patterns;
	patterns.push_back(Pattern("../Ressources/models/modelUC3.jpg",1500, 228));
	patterns.push_back(Pattern("../Ressources/models/power_supply.jpg", 170, 228));
	Scene scene1("../Ressources/views/launcher_test.jpg");
	Corners corner;
	Mat final_img = scene1.init_an_image(Pattern("../Ressources/models/modelUC3.jpg"));

	for (unsigned int i = 0; i < patterns.size(); i++) {

		scene1.matche_scene(patterns[i]);
		//scene1.show_matches(patterns[i], final_img);
		scene1.init_before_search( patterns[i]);

		while (scene1.searchPattern(final_img, corner, patterns[i],0));
	}
	//imshow("final image",final_img);
	std::cout << scene1.patterns.size() << std::endl;
	for (unsigned int i = 0; i < scene1.patterns.size(); i++) {
		scene1.patterns[i].printComponent();
	}

	waitKey(0);
}
