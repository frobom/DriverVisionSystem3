#ifndef DETECTION_DETECTEDRESULT_H
#define DETECTION_DETECTEDRESULT_H

#include "stdafx.h"
#include "opencv2/highgui.hpp"

namespace detection {
	using namespace std;
	using namespace cv;
	class DetectedResult
	{
		public:
			DetectedResult();
			~DetectedResult();
			static vector<Vec4i> detectedLines;
			static vector<Rect> detectedStopSign;
			static vector<Rect> detectedBackView;

	};

}
#endif