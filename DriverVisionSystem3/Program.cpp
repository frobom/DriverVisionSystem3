// DriverVisionSystem3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\highgui\highgui.hpp"
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	Mat image;
	image = imread("images\\dory01.jpg", 1);
	if (!image.data)
	{
		printf("No image data \n");
		return -1;
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);
	waitKey(0);
}

