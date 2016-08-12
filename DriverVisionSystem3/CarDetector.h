#ifndef DETECTION_CARDETECTOR_H
#define DETECTION_CARDETECTOR_H

#include "stdafx.h"
#include "opencv2\highgui.hpp"
#include "opencv2\opencv.hpp"

namespace detection {
	using namespace std;
	using namespace cv;
	static Mat gray;
	static vector<Rect> backview;
	static CascadeClassifier backviewDetector;
	class CarDetector
	{
	public:
		CarDetector();
		~CarDetector();

		static vector<Rect> detectBackView(Mat);

	private:

	};

}

#endif