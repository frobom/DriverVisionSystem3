#include "stdafx.h"
#include "RoadSignDetector.h"

using namespace detection;

RoadSignDetector::RoadSignDetector()
{
}

RoadSignDetector::~RoadSignDetector()
{
}

vector<Rect> RoadSignDetector::detectStopSign(Mat frame) {
	namedWindow("Stop Sign Detection", WINDOW_AUTOSIZE);

	CascadeClassifier stopSignDetector;
	stopSignDetector.load("classifiers\\stopsign_cascade0408.xml");

	vector<Rect> stopSign;

	if (stopSignDetector.empty()) {
		printf("Empty mode!!");
		return stopSign;
	}

	//Detect stop signs
	stopSignDetector.detectMultiScale(frame, stopSign, 1.1, 30, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(200, 200));

	return stopSign;
}