#ifndef DETECTION_SIDEWALLDETECTOR_H
#define DETECTION_SIDEWALLDETECTOR_H

#include "stdafx.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>   
using namespace cv;

using namespace std;
namespace detection {

	class SideWallDetector
	{
	public:
		SideWallDetector();
		~SideWallDetector();
		static vector<Point> detectSideWall(Mat image, bool leftWall);
	private:
		static Point findVanishPt(Mat image);
		static void draw2DegreeLineToRect(Point vanishPoint, vector<Rect>& rects, vector<Point>& existLines);
		static void draw15DegreeLineFromExistingLine(Point vanishPoint, vector<Point>& existLines, vector<Point>& imaginaryLines);
		static void getlineCountsBetweenExitAndImaginaryLine(Point vanishPoint, vector<Point>& existLines, vector<Point>& imaginaryLines, vector<int>& lineCounts);
		static bool intersection(Point o1, Point p1, Point o2, Point p2, Point &intersection);
	};
}

#endif