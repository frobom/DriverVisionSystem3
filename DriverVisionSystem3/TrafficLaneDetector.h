#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
namespace detection {
	class TrafficLaneDetector{
	public :
		TrafficLaneDetector();
		~TrafficLaneDetector();
		static vector<Vec4i> lineDetector(Mat image);
	};
}