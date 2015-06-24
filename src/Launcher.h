/*
 * Launcher.h
 *
 *  Created on: 24 juin 2015
 *      Author: manu
 */

#ifndef SRC_LAUNCHER_H_
#define SRC_LAUNCHER_H_

#include <string>
#include <vector>
#include "Composant.h"

class Launcher {
public:
	Launcher();
	Launcher(std::string name): name(name){}
	virtual ~Launcher();
private:
	std::string name;
	std::vector<Composant> composants;
};

#endif /* SRC_LAUNCHER_H_ */
