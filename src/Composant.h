/*
 * Composant.h
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#ifndef SRC_COMPOSANT_H_
#define SRC_COMPOSANT_H_


#include <string>
#include "Corners.h"

using namespace std;

class Composant {
public:
	Composant();
	Composant(string name,Corners corners):name(name),corners(corners){}
	void printComponent();
	virtual ~Composant();
private:
	string name;
	Corners corners;

};

#endif /* SRC_COMPOSANT_H_ */
