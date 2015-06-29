/*
 * Composant.cpp
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#include "Composant.h"

Composant::Composant() {
	// TODO Auto-generated constructor stub

}

Composant::~Composant() {
	// TODO Auto-generated destructor stub
}

void Composant::printComponent(){
	std::cout << "composant : " << name << std::endl;
	corners.print_corners();
}

