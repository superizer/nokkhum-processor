/*
 * recorder_attribute..cpp
 *
 *  Created on: Jul 21, 2011
 *      Author: boatkrap
 */

#include "recorder_attribute.hpp"

namespace nokkhum {

RecorderAttribute::RecorderAttribute(std::string name, std::string directory, int width, int height)
: ImageProcessorAttribute(name), directory(directory), width(width), height(height){
	// TODO Auto-generated constructor stub

}

RecorderAttribute::~RecorderAttribute() {
	// TODO Auto-generated destructor stub
}

    std::string RecorderAttribute::getDirectory() const
    {
        return directory;
    }

    int RecorderAttribute::getHeight() const
    {
        return height;
    }

    int RecorderAttribute::getWidth() const
    {
        return width;
    }

    void RecorderAttribute::setDirectory(std::string directory)
    {
        this->directory = directory;
    }

    void RecorderAttribute::setHeight(int height)
    {
        this->height = height;
    }

    void RecorderAttribute::setWidth(int width)
    {
        this->width = width;}

} /* namespace nokkhum */
