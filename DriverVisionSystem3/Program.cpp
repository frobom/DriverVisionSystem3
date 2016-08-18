// DriverVisionSystem3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RoadSignDetector.h"
#include "DetectedResult.h"
#include "opencv2/imgproc.hpp"
#include "CarDetector.h"
#include "TrafficLaneDetector.h"
#include "SideWallDetector.h"

using namespace std;
using namespace cv;
using namespace detection;

void drawRectangle(vector<Rect>, Mat);
void drawCircle(vector<Rect>, Mat);
void drawLine(vector<Vec4i>, Mat);

const int noOfPoints = 4;
Point rook_points[1][noOfPoints];
Point old_rook_points[1][noOfPoints];
const Point* ppt[1] = { rook_points[0] };
int npt[] = { noOfPoints };

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	if (argc != 3) {
		cout << "Invalid Arguments. There must be 2 arguments." << endl;
		system("PAUSE");
		return -1;
	}
	VideoCapture cap;

	if (string(argv[1]) == "-v") {
		cap = VideoCapture(argv[2]);
	}

	if (string(argv[1]) == "-c") {
		cap = VideoCapture(0);
	}

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//double fps = cap.get(CV_CAP_PROP_POS_MSEC); //get the frames per seconds of the video

	//cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	double frameCount = cap.get(CAP_PROP_FRAME_COUNT);

	cout << "Frame Count : " << frameCount;

	while (1)
	{
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		double currentFrame = cap.get(CV_CAP_PROP_POS_FRAMES);
		cout << "Current Frame : " << currentFrame;

		try {
			
			DetectedResult::detectedStopSigns = RoadSignDetector::detectStopSign(frame);
			DetectedResult::detectedLines = TrafficLaneDetector::lineDetector(frame);
			DetectedResult::detectedCarBackViews = CarDetector::detectBackView(frame);
			
			//vector<Point> sideWallPoints = detection::SideWallDetector::detectSideWall(frame, true);

			drawCircle(DetectedResult::detectedStopSigns, frame);
			drawRectangle(DetectedResult::detectedCarBackViews, frame);
			drawLine(DetectedResult::detectedLines, frame);

			//drawPloygon(sideWallPoints, frame);

			imshow("MyVideo", frame); //show the frame in "MyVideo" window

			if (waitKey(1) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
			{
				cout << "esc key is pressed by user" << endl;
				break;
			}
		}
		catch (ClassifierException &ce) {
			cerr << ce.what() << endl;
			system("PAUSE");
			break;
		}

	}
	VideoCapture();

	return 0;
}

void loadClassifiers() {
	printf("stop sign classifier loading.....\n");
	RoadSignDetector::stopSignDetector.load("E:\\Java\\DriverVisionSystem3\\DriverVisionSystem3\\classifiers\\stopsign_cascade0408.xml");

	if (RoadSignDetector::stopSignDetector.empty()) {
		throw ClassifierException();
	}
}

void drawRectangle(vector<Rect> resultVector, Mat frame) {
	for (int i = 0; i < resultVector.size(); i++) {
		Point pt1(resultVector[i].x, resultVector[i].y);
		Point pt2(resultVector[i].x + resultVector[i].width,
			resultVector[i].y + resultVector[i].height);

		// Draw a rectangle around the detected stop sign
		rectangle(frame, pt1, pt2, Scalar(255, 255, 255), 2);
	}
}

void drawCircle(vector<Rect> resultVector, Mat frame) {
	for (int i = 0; i < resultVector.size(); i++) {
		Point center((resultVector[i].x + resultVector[i].width) / 2, (resultVector[i].y + resultVector[i].height) / 2);

		// Draw a circle 
		circle(frame, center, 30, Scalar(0, 0, 255), 1, 8);
	}
}

void drawLine(vector<Vec4i> lines, Mat frame) {
	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		// Draw a line 
		line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 220, 0), 2, 8);
	}
}

void drawPloygon(vector<Point> points, Mat frame) {
	// draw wall
	Mat wall(frame.rows, frame.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat dst(frame.rows, frame.cols, CV_8UC3);

	if (points.size() == 4) {

		for (int i = 0; i < noOfPoints; i++) {
			rook_points[0][i] = points.at(i);
		}

		fillPoly(wall, ppt, npt, 1, Scalar(0, 0, 255), 8);
	}

	// end draw wall

	addWeighted(frame, 0.8, wall, (1 - 0.5), 0.0, dst);
}



