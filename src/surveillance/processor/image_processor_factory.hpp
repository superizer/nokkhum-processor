/*
 * image_processor_factory.hpp
 *
 *  Created on: Aug 12, 2011
 *      Author: boatkrap
 */

#ifndef NOKKHUM_IMAGE_PROCESSOR_FACTORY_HPP_
#define NOKKHUM_IMAGE_PROCESSOR_FACTORY_HPP_

#include <vector>

#include "image_processor.hpp"
#include "../../config/image_processor_property.hpp"
#include "../../util/cv_mat_queue.hpp"

namespace nokkhum {

class ImageProcessorFactory {
public:
	ImageProcessorFactory();
	virtual ~ImageProcessorFactory();

	ImageProcessor* getImageProcessor(ImageProcessorProperty* ipp, CvMatQueue* cv_mat_queue = nullptr);
	std::vector<ImageProcessor*> getImageProcessorPool(ImageProcessorProperty* ipp, MultipleMatQueue &mmq);

	void getImageProcessorFromVector(std::vector<ImageProcessor*> &image_processor_pool, std::vector<ImageProcessorProperty*> &ippv, ImageProcessor *parent_image_processor);
};

} /* namespace nokkhum */
#endif /* NOKKHUM_IMAGE_PROCESSOR_FACTORY_HPP_ */
