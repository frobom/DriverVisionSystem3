#include "stdafx.h"
#include "CarDetector.h"
using namespace std;
using namespace detection;

detection::CarDetector::CarDetector(){}

detection::CarDetector::~CarDetector(){}

vector<Rect> detection::CarDetector::detectBackView(Mat frame){

	cvtColor(frame, gray, CV_BGR2GRAY);

	Mat resize_image(cvRound(frame.rows), cvRound(frame.cols), CV_8UC1);

	resize(gray, resize_image, resize_image.size(), 0, 0, INTER_LINEAR);

	equalizeHist(resize_image, resize_image);

	backviewDetector.load("classifiers\\carBackView_cascade.xml");

	if (backviewDetector.empty()) {

		cout << "Empty CascadeClassifier XML file.\n";

	}

	backviewDetector.detectMultiScale(resize_image, backview, 1.1, 60, 0 | CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(150, 150));

	return backview;

}
