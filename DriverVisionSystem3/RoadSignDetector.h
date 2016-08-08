#ifndef DETECTION_ROADSIGNDETECTOR_H
#define DETECTION_ROADSIGNDETECTOR_H

#include "stdafx.h"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"

namespace detection {
	using namespace std;
	using namespace cv;
	class RoadSignDetector
	{
	public:
		RoadSignDetector();
		~RoadSignDetector();

		static vector<Rect> detectStopSign(Mat);

	private:

	};

}

#endif