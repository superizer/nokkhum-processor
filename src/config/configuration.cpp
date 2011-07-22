/*
 * configuration.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: boatkrap
 */

#include "configuration.hpp"

namespace nokkhum {

Configuration::Configuration() {
	this->camera_property = nullptr;
	this->image_processor_property = nullptr;
}

Configuration::~Configuration() {
	delete this->camera_property;
	delete this->image_processor_property;

	this->camera_property = nullptr;
	this->image_processor_property = nullptr;
}

CameraProperty *Configuration::getCameraProperty() const {
	return camera_property;
}

ImageProcessorProperty *Configuration::getImageProcessorProperty() const {
	return image_processor_property;
}

} /* namespace nokkhum */