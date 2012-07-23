/*
 * cv_mat_queue.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: boatkrap
 */

#include "cv_mat_queue.hpp"

#include <iostream>

#include <glog/logging.h>

#include <thread>
#include <chrono>

namespace nokkhum {

CvMatQueue::CvMatQueue() {
	// TODO Auto-generated constructor stub
//	std::cout<<"construct "<<" this->"<<this <<std::endl;

}

CvMatQueue::~CvMatQueue() {
	// TODO Auto-generated destructor stub
}

void CvMatQueue::push(cv::Mat mat) {


	queue_mutex.lock();
	cv_mat_queue.push(mat);
	queue_mutex.unlock();

	if (this->size() > 300 ){
		if(this->drop){
			this->drop = false;
			LOG(INFO) << "thread: " << std::this_thread::get_id() << " cv_mat_queue drop";
			this->pop();
		}
		else{
			this->drop = true;
		}
	}

	if (this->size() > 100 ){
		LOG(INFO) << "thread: " << std::this_thread::get_id() << " cv_mat_queue size: " << this->size();
		std::chrono::milliseconds dura( 500 );
		std::this_thread::sleep_for( dura );
	}



}

cv::Mat CvMatQueue::pop() {
	queue_mutex.lock();
	cv::Mat mat = cv_mat_queue.front().clone();
	cv_mat_queue.pop();
	queue_mutex.unlock();
	return mat;
}

bool CvMatQueue::empty() {
	return cv_mat_queue.empty();
}

unsigned int CvMatQueue::size() {
	return cv_mat_queue.size();
}

}
