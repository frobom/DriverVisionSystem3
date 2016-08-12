#include "stdafx.h"
#include "SideWallDetector.h"

detection::SideWallDetector::SideWallDetector(){
}

detection::SideWallDetector::~SideWallDetector(){
}

Point detection::SideWallDetector::findVanishPt(Mat image) {

	Mat gray;
	Point newVanishPoint;

	cvtColor(image, gray, CV_BGR2GRAY);
	//imshow("gray", gray);

	GaussianBlur(gray, gray, cv::Size(13, 13), 0, 0);
	//imshow("blur", gray);

	Canny(gray, gray, 75, 100, 7);
	//imshow("canny", gray);

	//detect lines
	vector<Vec2f> houghlines;
	HoughLines(gray, houghlines, 1, CV_PI / 180, 200, 0, 0);

	//draw lines
	vector<Vec2f> drawLines;
	Point p1, p2, p3, p4;
	Point rook_points[1][4];
	vector<Point> drawLinesStart;
	vector<Point> drawLinesEnd;

	for (size_t i = 0; i < houghlines.size(); i++) {

		float rho = houghlines[i][0], theta = houghlines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;

		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		if (pt1.y != pt2.y && (pt1.y - pt2.y > 400 || pt2.y - pt1.y > 400)) {
			drawLinesStart.push_back(Point(pt1.x, pt1.y));
			drawLinesEnd.push_back(Point(pt2.x, pt2.y));
			//line(image, Point(pt1.x, pt1.y), Point(pt2.x, pt2.y), Scalar(0, 255, 0), 3, CV_AA);
		}
	}

	vector<Point> vanishPoints;

	for (int i = 0; i < drawLinesStart.size(); i++) {

		for (int j = i + 1; j < drawLinesStart.size(); j++) {

			Point intersetPoint;
			intersection(drawLinesStart.at(i), drawLinesEnd.at(i), drawLinesStart.at(j), drawLinesEnd.at(j), intersetPoint);

			//cout << "intersection point " << r.x << ", " << r.y << endl;

			if (!(intersetPoint.x == 0 && intersetPoint.y == 0)) {

				vanishPoints.push_back(intersetPoint);
			}
		}
	}

	int sumX = 0;
	int sumY = 0;

	for (int i = 0; i < vanishPoints.size(); i++){

		sumX = vanishPoints.at(i).x + sumX;
		sumY = vanishPoints.at(i).y + sumY;

	}

	if (vanishPoints.size() > 0) {

		int newVanishPointX = sumX / vanishPoints.size();
		int newVanishPointY = sumY / vanishPoints.size();

		newVanishPoint.x = newVanishPointX;
		newVanishPoint.y = newVanishPointY;
	}

	return newVanishPoint;

}

void detection::SideWallDetector::draw2DegreeLineToRect(Point vanishPoint, vector<Rect>& rects, vector<Point>& existLines) {

	for (int i = 0; i < rects.size(); i++) {

		if (rects.at(i).y > vanishPoint.y){

			Point up, down;

			up.x = rects.at(i).x + (((rects.at(i).x + rects.at(i).width) - rects.at(i).x) / 2);
			up.y = rects.at(i).y;

			down.x = up.x;
			down.y = rects.at(i).y + rects.at(i).height;

			//line(img, up, down, Scalar(0, 255, 0), 3, CV_AA);
			//line(img, vanishPoint, Point(down.x, up.y), Scalar(0, 255, 0), 1, CV_AA);
			//line(img, vanishPoint, Point(down.x, down.y), Scalar(0, 255, 0), 1, CV_AA);

			existLines.push_back(Point(down.x, up.y));
			existLines.push_back(Point(down.x, down.y));

			for (int angle = 2; angle < 180; angle = angle + 2) {

				for (int i = up.y; i < down.y; i++){

					Point currentPoint, toDraw;
					int length;

					currentPoint.x = down.x;
					currentPoint.y = i;

					length = floor(norm(vanishPoint - currentPoint));

					toDraw.x = (int)round(vanishPoint.x + length * cos(angle * CV_PI / 180.0));
					toDraw.y = (int)round(vanishPoint.y + length * sin(angle * CV_PI / 180.0));

					if ((toDraw.x == currentPoint.x || toDraw.x == currentPoint.x + 1 || toDraw.x == currentPoint.x - 1) && toDraw.y == currentPoint.y) {

						//line(img, vanishPoint, currentPoint, Scalar(0, 255, 0), 1, CV_AA);

						existLines.push_back(currentPoint);

						i = down.y;
					}
				}
			}
		}
	}
}

void detection::SideWallDetector::draw15DegreeLineFromExistingLine(Point vanishPoint, vector<Point>& existLines, vector<Point>& imaginaryLines) {

	int length = 500;

	for (int i = 0; i < existLines.size(); i++){//exitLines.size()

		//length = ceil(norm(existLines.at(i) - vanishPoint));

		float calDegree;

		if (existLines.at(i).x > vanishPoint.x) {

			float h = existLines.at(i).y - vanishPoint.y; //current.y - vanishingpoint.y;
			float b = existLines.at(i).x - vanishPoint.x; //current.x - vanishingpoint.x ;
			calDegree = (h / b);

		}
		else {

			float h = existLines.at(i).y - vanishPoint.y; //current.y - vanishingpoint.y;
			float b = vanishPoint.x - existLines.at(i).x; //vanishingPoint.x - current.x;
			calDegree = (b / h);

		}

		int degree = atan(calDegree) * 180 / CV_PI;

		if (existLines.at(i).x > vanishPoint.x) {

			degree = degree + 15;
		}
		else {

			degree = degree + 90 + 15;
		}

		//cout << "Calculate degree " << degree << endl;

		Point imaginaryLine;
		imaginaryLine.x = (int)round(vanishPoint.x + length * cos(degree * CV_PI / 180.0));
		imaginaryLine.y = (int)round(vanishPoint.y + length * sin(degree * CV_PI / 180.0));

		imaginaryLines.push_back(imaginaryLine);
	}
}

void detection::SideWallDetector::getlineCountsBetweenExitAndImaginaryLine(Point vanishPoint, vector<Point>& existLines, vector<Point>& imaginaryLines, vector<int>& lineCounts) {

	for (int i = 0; i < existLines.size(); i++) {

		int count = 0;

		for (int j = i; j < existLines.size(); j++) {

			Point intersetion;
			intersetion.x = 0;
			intersetion.y = 0;
			//line(img, existLines.at(i), imaginaryLines.at(i), Scalar(255, 255, 255), 1, CV_AA);
			intersection(existLines.at(i), imaginaryLines.at(i), vanishPoint, existLines.at(j), intersetion);
			if (!(intersetion.x == 0 && intersetion.y == 0) &&
				((intersetion.x<existLines.at(i).x && intersetion.x>imaginaryLines.at(i).x) ||
				(intersetion.y>existLines.at(i).y && intersetion.y<imaginaryLines.at(i).y))) {
				count++;
			}
		}

		lineCounts.push_back(count);
	}
}

bool detection::SideWallDetector::intersection(Point o1, Point p1, Point o2, Point p2,
	Point &r)
{
	Point x = o2 - o1;
	Point d1 = p1 - o1;
	Point d2 = p2 - o2;

	float cross = d1.x*d2.y - d1.y*d2.x;
	if (abs(cross) < /*EPS*/1e-8) {
		return false;
	}

	double t1 = (x.x * d2.y - x.y * d2.x) / cross;
	r = o1 + d1 * t1;
	return true;
}

vector<Point> detection::SideWallDetector::detectSideWall(Mat image, bool left) {


	vector<Point> polyPoints;
	Point oldVanishPoint;
	Point vanishPoint;

		vanishPoint = findVanishPt(image);

		if ((vanishPoint.x < ceil((image.cols * 2) / 3) && vanishPoint.x > ceil((image.cols * 1) / 3)) // < 3/5 , > 2/5
			&& (vanishPoint.y < ceil((image.rows * 2) / 3))) {

			oldVanishPoint = vanishPoint;
		}
		else {

			vanishPoint = oldVanishPoint;
		}

		//-------------rectangle-----------------//

		Mat roi(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

		if (!(vanishPoint.x == 0 && vanishPoint.y == 0)) {

			vector <Rect> rects;

			CascadeClassifier guardRailDetector;

			if (left == true) {
				guardRailDetector.load("E:\\Myat Myo Lwin\\car_back_view_200_339.xml");
			}
			else {
				guardRailDetector.load("E:\\Myat Myo Lwin\\guard_rail_right.xml");
			}

			guardRailDetector.detectMultiScale(image, rects, 1.1, 30, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));


			//-------------------------find rail-----------------------------//

			// Draw all rectangle and find 2 degree lines
			vector<Point> existLines;
			draw2DegreeLineToRect(vanishPoint, rects, existLines);

			//Finding 15 degree line from exit
			vector<Point> imaginaryLines;
			draw15DegreeLineFromExistingLine(vanishPoint, existLines, imaginaryLines);

			// finding line between exist line and 15 degree line
			vector<int> lineCounts;
			getlineCountsBetweenExitAndImaginaryLine(vanishPoint, existLines, imaginaryLines, lineCounts);

			int maxCount = 0;
			for (int i = 0; i < lineCounts.size(); i++) {

				if (maxCount < lineCounts.at(i)) {

					maxCount = lineCounts.at(i);
				}
			}

			//search max line count area

			for (int i = 0; i < lineCounts.size(); i++) {

				if (lineCounts.at(i) == maxCount) {

					polyPoints.push_back(vanishPoint);
					polyPoints.push_back(Point(vanishPoint.x + 1, vanishPoint.y));
					polyPoints.push_back(existLines.at(i));//longExitLine;
					polyPoints.push_back(imaginaryLines.at(i));

					i = lineCounts.size();
				}
			}
		}
		
		return polyPoints;
}