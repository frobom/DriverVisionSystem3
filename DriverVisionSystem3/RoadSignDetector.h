#ifndef DETECTION_ROADSIGNDETECTOR_H
#define DETECTION_ROADSIGNDETECTOR_H

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include <exception>

namespace detection {
	static int methodCallCount;

	using namespace std;
	using namespace cv;	

	struct ClassifierException : public exception
	{
		const char * what() const throw ()
		{
			return "Classifier error!!";
		}
	};

	class RoadSignDetector
	{
	public:
		RoadSignDetector();
		~RoadSignDetector();
		static CascadeClassifier stopSignDetector;
		static vector<Rect> detectStopSign(Mat);

	};
}

#endif