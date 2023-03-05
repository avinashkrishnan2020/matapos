#define DEBUG

#include "camera/camera.h"

#include <thread>
#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <memory>
#include <thread>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>


Camera::Camera(int deviceId, int apiId) {
	deviceId = deviceId;
	apiId = apiId;
}

Camera::Camera() {
	deviceId = 0;
	apiId = cv::CAP_ANY;
	
	#ifdef DEBUG
		std::cout << "deviceId: " << deviceId << std::endl;
		std::cout << "apiId: " << apiId << std::endl;
	#endif	
}

void Camera::qrDecoderCallback(cv::Mat frame) {
	cv::QRCodeDetector qrDecoder = cv::QRCodeDetector();
	std::string data = qrDecoder.detectAndDecode(frame);
	if(data.length() > 0) {
		std::cout << "Decoded QR code = " << data;
	} 
}

void Camera::runCamera() {
	
	cv::namedWindow("Video Player");
	
	while(isRunning) {
		cv::Mat frame;
		videoCapture.read(frame);
		if(frame.empty()) {

			#ifdef DEBUG
				std::cerr << std::endl << "empty frame!" << std::endl;
			#endif
		}

		imshow("Video Player", frame);
		//qrDecoderCallback(frame);

	}
}


void Camera::start() {
	
	videoCapture.open(deviceId, apiId);
	

	if(videoCapture.isOpened()) {
		#ifdef DEBUG
			std::cout << std::endl << "Camera opened successfully!" << std::endl;
		#endif
		
		isRunning = 1;
	}
	else {
		#ifdef DEBUG
			std::cerr << std::endl << "Error occured...Unable to open camera..." << std::endl;
		#endif

		return;
	}
	
	// run camera in a separate thread
	cameraThread = std::thread(&Camera::runCamera, this);

}

void Camera::stop() {
	isRunning = 0;
}

Camera::~Camera() {

}