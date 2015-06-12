/*
 * DirectoryHandler.cpp
 *
 *  Created on: 9 juin 2015
 *      Author: manu
 */
#include "DirectoryHandler.h"

string StoreDirectory::toString() {
	string s;
	s = s + folder.filename().string() + " :\n";
	for (unsigned int i = 0; i < files.size(); i++) {
		s += "_ " + files[i].filename().string() + "\n";
	}
	return s;
}

void DirectoryHandler::get_paths(const fs::path& root) {
	if (!fs::exists(root) || !fs::is_directory(root))
		return;

	//folders = vector<StoreDirectory*>();
	fs::recursive_directory_iterator it(root);
	fs::recursive_directory_iterator endit;
	StoreDirectory sd(root);

	while (it != endit) {
		// if we are in the root directory
		if (it->path().parent_path() == root) {
			// when the file storeDirectory if full we clear it in the folders
			if (!sd.files.empty()) {
				folders.push_back(sd);
				sd.files.clear();
			}
			// if we find a directory we change the name of the current storeDirectory
			if (fs::is_directory(*it)) {
				sd.folder = it->path();
			}
		}
		// If we're not in the root directory we push files inside the storeDirectory
		else {

			if (fs::is_regular_file(*it)) {
				sd.files.push_back(it->path());
			}
		}
		++it;
	}
	//folders.push_back(*sd);
}

void save_one(fs::path,int index){

}
void save_all(fs::path);
void load_one(fs::path, int index);
void load_all(fs::path);



void DirectoryHandler::printPaths() {
	for (unsigned int i = 0; i < folders.size(); i++) {
		std::cout << folders[i].toString() << std::endl;
	}
}





















StoreDirectory::StoreDirectory(const fs::path& path) {
	folder = fs::path(path);
	files = vector<fs::path>();
}




DirectoryHandler::DirectoryHandler() {
}
DirectoryHandler::~DirectoryHandler() {

}


