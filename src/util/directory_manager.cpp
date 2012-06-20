/*
 * directory_manager.cpp
 *
 *  Created on: Oct 25, 2011
 *      Author: boatkrap
 */

#include "directory_manager.hpp"
#include <iomanip>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <glog/logging.h>
namespace nokkhum {

DirectoryManager::DirectoryManager(std::string root_directory, std::string type) :
		root_directory(root_directory), type(type) {

	std::ostringstream oss;
	boost::posix_time::ptime current_time =
			boost::posix_time::microsec_clock::local_time();
	oss << root_directory << "/" << current_time.date().year() << std::setw(2)
			<< std::setfill('0') << (int) ((current_time.date().month()))
			<< std::setw(2) << std::setfill('0') << current_time.date().day();
	if (type == "image") {
		oss << "/" << "images" << "/"
				<< std::setw(2) << std::setfill('0') << current_time.time_of_day().hours() << "-"
				<< std::setw(2) << std::setfill('0') << current_time.time_of_day().minutes();
	} else if (type == "video") {
		oss << "/" << "video";
	} else {
		oss << "/" << "unknow_type";
	}

	directory_path = boost::filesystem::path(oss.str());
}

DirectoryManager::~DirectoryManager() {
	// TODO Auto-generated destructor stub
}

bool DirectoryManager::checkAndCreate() {
	if (boost::filesystem::exists(directory_path)) {
		return true;
	}
	if (boost::filesystem::create_directories(directory_path)) {
		LOG(INFO) << "Create directory name: " << directory_path;
		return true;
	}
	return false;
}

std::string DirectoryManager::getDirectoryName() {
	return this->directory_path.native();

}

} /* namespace nokkhum */