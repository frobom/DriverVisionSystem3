// DriverVisionSystem3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "RoadSignDetector.h"
#include "DetectedResult.h"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace detection;

int _tmain(int argc, _TCHAR* argv[])
{
	/*Mat image;
	image = imread("images\\stopSign2.jpg", 1);
	if (!image.data)
	{
		printf("No image data \n");
		return -1;

	}*/	

	VideoCapture cap("videos\\Learn to drive with Rico Williams.MP4"); // open the video file for reading

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//double fps = cap.get(CV_CAP_PROP_POS_MSEC); //get the frames per seconds of the video

	//cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	double frameCount = cap.get(CAP_PROP_FRAME_COUNT);
	double currentFrame = cap.get(CV_CAP_PROP_POS_FRAMES);

	cout << "Frame Count : " << frameCount;

	while (1)
	{
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video
		currentFrame = cap.get(CV_CAP_PROP_POS_FRAMES);
		cout << "Current Frame : " << currentFrame;

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		//DetectedResult::detectedStopSign = RoadSignDetector::detectStopSign(frame);
		//vector<Rect> stopSign = DetectedResult::detectedStopSign;
		vector<Rect> stopSign = RoadSignDetector::detectStopSign(frame);
		DetectedResult::detectedStopSigns = stopSign;
		for (int i = 0; i < stopSign.size(); i++) {
			Point pt1(stopSign[i].x, stopSign[i].y);
			Point pt2(stopSign[i].x + stopSign[i].width,
				stopSign[i].y + stopSign[i].height);

			// Draw a rectangle around the detected stop sign
			rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2);
			/*Point pt(stopSign[i].x, stopSign[i].y);
			rectangle(image, pt1, pt2, Scalar(0, 255, 0), CV_FILLED);*/
			putText(frame, "Stop Sign", pt1, FONT_HERSHEY_PLAIN, 1.3,
				Scalar(255, 0, 0), 2);
		}

		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (waitKey(1) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	VideoCapture();

	/*namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);*/
	//waitKey(0);

	return 0;
}

