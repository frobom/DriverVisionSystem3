#include "stdafx.h"
#include "RoadSignDetector.h"
#include <iostream>

using namespace detection;

CascadeClassifier RoadSignDetector::stopSignDetector;

RoadSignDetector::RoadSignDetector()
{
}

RoadSignDetector::~RoadSignDetector()
{
}


vector<Rect> RoadSignDetector::detectStopSign(Mat frame) {
	printf("stop sign classifier loading.....\n");
	stopSignDetector.load("E:\\Java\\DriverVisionSystem3\\DriverVisionSystem3\\classifiers\\stopsign_cascade0408.xml");

	if (stopSignDetector.empty()) {
		throw ClassifierException();
	}

	namedWindow("Stop Sign Detection", WINDOW_AUTOSIZE);

	vector<Rect> stopSign;

	//Detect stop signs
	stopSignDetector.detectMultiScale(frame, stopSign, 1.1, 30, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(200, 200));

	return stopSign;
}




