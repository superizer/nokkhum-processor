/*
 * video_recorder_attribute.cpp
 *
 *  Created on: Jul 25, 2011
 *      Author: boatkrap
 */

#include "video_recorder_attribute.hpp"

namespace nokkhum {

VideoRecorderAttribute::VideoRecorderAttribute(std::string name,
		std::string directory, int width, int height, int fps) :
		RecorderAttribute(name, directory, width, height), fps(fps) {
	// TODO Auto-generated constructor stub
	this->record_motion = false;

}

VideoRecorderAttribute::VideoRecorderAttribute(std::string name,
		std::string directory, int width, int height, int fps,
		bool record_motion) :
		RecorderAttribute(name, directory, width, height), fps(fps),
		record_motion(record_motion)
		{

}

VideoRecorderAttribute::~VideoRecorderAttribute() {
	// TODO Auto-generated destructor stub
}

int VideoRecorderAttribute::getFps() const {
	return fps;
}

void VideoRecorderAttribute::setFps(int fps) {
	this->fps = fps;
}

bool VideoRecorderAttribute::isRecordMotion() const {
	return record_motion;
}

} /* namespace nokkhum */
