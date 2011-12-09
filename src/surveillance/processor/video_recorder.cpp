/*
 * video_recorder.cpp
 *
 *  Created on: May 4, 2011
 *      Author: boatkrap
 */

#include "video_recorder.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <exception>

#include <opencv2/highgui/highgui.hpp>
#include <glog/logging.h>

#include "../../video/cv_video_writer.hpp"

#include "../../util/directory_manager.hpp"

#include <sys/time.h>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace nokkhum {

VideoRecorder::VideoRecorder(CvMatQueue& input_image_queue) :
		ImageProcessor("VDO Recorder", input_image_queue) {
	this->running = false;
	this->writer = nullptr;

	this->period = 10;
	this->timer = nullptr;

	// std::cout << "Construct video recorder without property" << std::endl;
}

VideoRecorder::VideoRecorder(CvMatQueue & input_image_queue,
		VideoRecorderAttribute *vrp) :
		ImageProcessor(vrp->getName(), input_image_queue) {
	this->running = false;
	this->directory = vrp->getDirectory();
	this->width = vrp->getWidth();
	this->height = vrp->getHeight();
	this->fps = vrp->getFps();
	this->period = 10;
	this->writer = nullptr;
	this->timer = nullptr;
}

VideoRecorder::~VideoRecorder() {
	delete this->writer;
	this->writer = nullptr;

	delete this->timer;
	this->timer = nullptr;
}

void VideoRecorder::start() {
	// this->getNewVideoWriter();
	this->startTimer();
	this->startRecord();
}

void VideoRecorder::stop() {
	Job::stop();
	this->stopTimer();
}

void VideoRecorder::getNewVideoWriter() {

	nokkhum::DirectoryManager dm (this->directory, "video");
	if(! dm.checkAndCreate()){

	}

	std::ostringstream oss;

	std::string name = this->getName();
	for (unsigned int i  = 0; i < name.length(); ++i){
		if (name.at(i) == ' ')
			name.at(i) = '_';
	}

	boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();

	oss << current_time.date().year() << "-"
			<< std::setw(2) << std::setfill('0') << (int)current_time.date().month() << "-"
			<< std::setw(2) << std::setfill('0') << current_time.date().day() << "-"
			<< std::setw(2) << std::setfill('0') << current_time.time_of_day().hours() << "-"
			<< std::setw(2) << std::setfill('0') << current_time.time_of_day().minutes() << "-"
			<< std::setw(2) << std::setfill('0') << current_time.time_of_day().seconds() << "-"
			<< std::setw(6) << std::setfill('0') << current_time.time_of_day().fractional_seconds() << "-"
			<< name << ".avi";

	writer_mutex.lock();
	delete this->writer;
	this->writer = nullptr;

	while(this->writer == nullptr){
		this->writer = new CvVideoWriter(oss.str(), dm.getDirectoryName(), this->width,
			this->height, this->fps);

		if(!this->writer->isAvailable()){
			delete this->writer;
			this->writer = nullptr;
		}
	}
	writer_mutex.unlock();

	this->filename = oss.str();
	LOG(INFO) << "get new video writer name: " << dm.getDirectoryName() << this->filename ;

}

void VideoRecorder::startRecord() {

	cv::Mat frame;
	//	cv::namedWindow("video", 1);

	//	std::cout << "Start record tread/running status: " << runnin//	time_t rawtime;

	//	std::cout<<"runing: "<<this->running<<std::endl;

	//	time(&rawtime);
	//	std::cout<<"start join timer:"<<ctime(&rawtime)<<std::endl;
		//this->timer.join();
	//	time(&rawtime);
	//	std::cout<<"start end join timer:"<<ctime(&rawtime)<<std::endl;g << " this-> "
	//			<< this << std::endl;

	while (running) {

		if (input_image_queue.empty()) {
			//			std::cout << "sleep .zZ" << std::endl;
			usleep(1000);
			continue;
		}
//		std::cout << "write to file " << this->filename << std::endl;

		frame = input_image_queue.pop();

		//		cv::imshow("video", frame);
		writer_mutex.lock();
		*writer << frame;
		writer_mutex.unlock();

		//		if (cv::waitKey(30) >= 0)
		//			break;
	}
//	std::cout<<"record stop"<<std::endl;
}

void VideoRecorder::startTimer() {
	// LOG(INFO) << "Start Timer";
	// timer = std::thread(&VideoRecorder::clock, this);
	// LOG(INFO) << "Timer RUN";
	this->timer = new RecordTimer(this, this->period);
	this->timer->start();
	// LOG(INFO) << this->getName() << " start Timer";
}

void VideoRecorder::stopTimer() {
	// LOG(INFO) << this->getName() << " stop Timer";
	try {
		if(this->timer != nullptr){
			this->timer->stop();
			// LOG(INFO) << this->getName() << " delete Timer";
			delete this->timer;
			this->timer = nullptr;
		}
	} catch (std::exception e) {
		LOG(INFO) << "exception in stop timer: " << e.what() << std::endl;
	}
	// LOG(INFO) << this->getName() << " end stop Timer";
}

bool VideoRecorder::is_writer_available(){
	if (this->writer)
		return true;
	else
		return false;
}

RecordTimer::RecordTimer(VideoRecorder *video_recorder, int period = 10) :
		video_recorder(video_recorder), period(period) {
	running = false;
}

RecordTimer::RecordTimer(){
	running = false;
	video_recorder = nullptr;
	period = 0;
}

RecordTimer::RecordTimer(const RecordTimer& rt){
	this->running = rt.running;
	this->period  = rt.period;
	this->video_recorder = rt.video_recorder;
}

RecordTimer& RecordTimer::operator = (const RecordTimer& rt){
	this->running = rt.running;
	this->period  = rt.period;
	this->video_recorder = rt.video_recorder;

	return *this;
}

RecordTimer::~RecordTimer(){
	// LOG(INFO) << "begin teminate timmer id: "<<this;
	if(!this->timer_thred.joinable())
		this->timer_thred.join();

	// LOG(INFO) << "teminate timmer id: "<<this;
}

void RecordTimer::start(){
	// LOG(INFO) << "start Clock :";
	running = true;
	timer_thred = boost::thread(&RecordTimer::clock, this);
	// LOG(INFO) << "end start Clock :";
}

void RecordTimer::stop(){
	// LOG(INFO) << "stop Clock :";
	running = false;
	timer_thred.interrupt();
	// LOG(INFO) << "end stop Clock :";
}

void RecordTimer::clock(){
	// LOG(INFO) << "new Clock :"<<this<<" name: "<<video_recorder->getName()<<" thread id: "<<std::this_thread::get_id();
	while (running) {
		// LOG(INFO) << "Clock working: "<<this<<" name: "<<video_recorder->getName();
		boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

		if (start_time.time_of_day().minutes() % this->period == 0) {
			video_recorder->getNewVideoWriter();
		}
		else if (!video_recorder->is_writer_available()){
			video_recorder->getNewVideoWriter();
		}

		boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();


		int sleep_time = (this->period
				- ((this->period + current_time.time_of_day().minutes()) % this->period)) * 60;

		sleep_time = sleep_time - current_time.time_of_day().seconds();

		if (sleep_time <= 120){
			// LOG(INFO) << "Clock sleep more time "<<sleep_time<<"s" <<" id: "<<this<<" name: "<<video_recorder->getName();
			sleep_time += (this->period*60);
		}

		// std::cout << "sleep ---> " << sleep_time << std::endl;
		// LOG(INFO) << "Clock sleep "<<sleep_time<<"s" <<" id: "<<this<<" name: "<<video_recorder->getName();
		sleep(sleep_time);

	}
	// LOG(INFO) << "Clock end: "<<this<<" name: "<<video_recorder->getName();
}

}
