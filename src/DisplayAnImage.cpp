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

void value_tester() {
	vector<Pattern> patterns;
	int baseValue = 1000;


	//patterns.push_back(Pattern("../Ressources/models/modelUC2.jpg",1500, 235));
	Scene scene1("../Ressources/views/beans1.jpg");
	Corners corner;
	Mat final_img = scene1.init_an_image(
			Pattern("../Ressources/views/beans1.jpg"));
	while (scene1.patterns.size() < 2 && baseValue < 2500) {
		scene1.patterns.clear();
		baseValue += 25;
		Pattern pattern("../Ressources/models/dustBeanBlack.jpg", baseValue, 140);
		//pattern.treshold = baseValue;
		scene1.matche_scene(pattern);
		scene1.show_matches(pattern, final_img);
		scene1.init_before_search(pattern);

		while (scene1.searchPattern(final_img, corner, pattern, 1))
			;
	}
	std::cout << "base value : " << baseValue << std::endl;
	imshow("final image", final_img);

}

/************************************************************************************************************/
// ----------------------------------------------- MAIN --------------------------------------------------- //
int main(int argc, char* argv[]) {

	//value_tester();

	 vector<Pattern> patterns;
	 //patterns.push_back(Pattern("../Ressources/models/dustBeanBlack.jpg", 1400, 140));
	 //patterns.push_back(Pattern("../Ressources/models/modelUC2.jpg",1500, 235));
	 patterns.push_back(Pattern("../Ressources/models/fullRacClear.jpg",400, 150));
	 Scene scene1("../Ressources/views/WithCookies.png");
	 Corners corner;
	 Mat final_img = scene1.init_an_image(Pattern("../Ressources/views/WithCookies.png"));

	 for (unsigned int i = 0; i < patterns.size(); i++) {
	 //std::cout << "detection of pattern " << i << std::endl;

	 scene1.matche_scene(patterns[i]);
	 scene1.show_matches(patterns[i], final_img);
	 scene1.init_before_search(patterns[i]);

	 while (scene1.searchPattern(final_img, corner, patterns[i],1));
	 }
	 imshow("final image",final_img);
	 for (unsigned int i = 0; i < scene1.patterns.size(); i++) {
	 scene1.patterns[i].printComponent();
	 }

	waitKey(0);
}
