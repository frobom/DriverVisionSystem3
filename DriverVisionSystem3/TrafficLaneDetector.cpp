#include "stdafx.h"
#include "TrafficLaneDetector.h"

using namespace cv;

vector<Vec4i> detection::TrafficLaneDetector::lineDetector(Mat image){
	vector<Vec4i> lines;
		Mat frame;
		//Mat roi = image(Rect(0, image.rows / 2, image.cols, image.rows / 2));
		Mat roi = image(Rect(0, image.cols / 3, image.cols - 1, image.rows - image.cols / 3));
		cvtColor(roi, frame, CV_BGR2GRAY);
		GaussianBlur(frame, frame, Size(7, 7), 0, 0);
		Canny(frame, frame, 150, 50, 3);
		HoughLinesP(frame, lines, 1, CV_PI / 180, 100, 50, 10);

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(roi, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 225, 0), 3, CV_AA);
		}
	return lines;
}