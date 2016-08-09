#include "stdafx.h"
#include "DetectedResult.h"

using namespace detection;

DetectedResult::DetectedResult(){

}

DetectedResult::~DetectedResult() {

}

vector<Vec4i> DetectedResult::detectedLines;
vector<Rect> DetectedResult::detectedStopSigns;
vector<Rect> DetectedResult::detectedCarBackViews;