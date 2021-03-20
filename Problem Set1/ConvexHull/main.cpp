#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<cmath>
using namespace cv;
using namespace std;
Mat img;
int thresh = 100;
RNG rng(12345);
vector<Point> cusConvexHull(vector<Point> & in);
uint32_t differenceSquared(const Point a, const Point b);
uint32_t orientation(const Point a, const Point b, const Point c);

int main( int argc, char** argv )
{
    int height = 500;
    int width = 500;

    img = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));

    vector<Point> randomPoints;
    int numberOfPoints = 20;
    for(int i = 0; i < numberOfPoints; i++){
        randomPoints.push_back(Point(rng.uniform(0,height), rng.uniform(0,width)));
    }

    for(auto pnt: randomPoints){
        // Display points
        circle(img, pnt, 0, Scalar(0, 0, 255), 5);
    }


    imshow( "Original Points", img );

    Mat builtInCV = img.clone();
    vector<Point> ocvHull;
    bool clockwise = true;
    convexHull(randomPoints, ocvHull, clockwise);
    const Point* ocvElementPoints[1] = { &ocvHull[0] };
    const int numberOfOCVPoints = (int)ocvHull.size();
    fillPoly(builtInCV, ocvElementPoints, &numberOfOCVPoints, 1, Scalar(0,255,0));
    imshow("Opencv ConvexHull", builtInCV);

    Mat customCH = img.clone();
    vector<Point> cusHull = cusConvexHull( randomPoints);

    const Point* cusElementPoints[1] = { &cusHull[0] };
    const int numberOfCusPoints = (int)cusHull.size();
    fillPoly(customCH, cusElementPoints, &numberOfCusPoints, 1, Scalar(255,0,0));
    imshow("Custom ConvexHull", customCH);
    for(auto pt: cusHull){
        std::cout << pt << std::endl;
    }

    //print out points to show Clockwise order


    waitKey();
    return 0;
}
vector<Point> cusConvexHull(vector<Point> & in)
{
    // Implementing Graham Scan, using resources and documentation from https://www.geeksforgeeks.org
    // Pick smallest y, smallest x is the tie-break
    int minIndex = 0;
    //int min = out.at(1).y;
    for(size_t i = 1; i < in.size(); i++){
        bool currentLessMin = in.at(i).y < in.at(minIndex).y;
        bool currentEqualMin = in.at(i).y == in.at(minIndex).y;
        if(currentLessMin || (currentEqualMin && in.at(i).x < in.at(minIndex).x)){
            minIndex = i;
        }
    }
    if(minIndex > 0){
        std::swap(in.at(0), in.at(minIndex));
    }

    // Sort points with respect to the bottom point
    std::sort(in.begin()+1, in.end(), [&](auto a, auto b){
        int ori = orientation(in.at(0), a, b);
        if(ori == 0){
            return differenceSquared(in.at(0), a) >= differenceSquared(in.at(0), b);
        }
        return ori > 1;
    });


    vector<Point> tmp;
    tmp.push_back(in.front());
    for(size_t i = 1; i < in.size(); i++){
        while(i < in.size()-1 && orientation(in.at(0), in.at(i), in.at(i+1)) == 0){
            i++;
        }
        tmp.push_back(in.at(i));
    }
    if(tmp.size() < 3){
        return vector<Point>();
    }

    vector<Point> out;
    out.push_back(tmp.at(0));
    out.push_back(tmp.at(1));
    out.push_back(tmp.at(2));


    for(size_t i = 3; i < tmp.size(); i++){
        while(orientation(out.at(out.size()-2), out.back(), tmp.at(i)) != 2){
            out.pop_back();
        }
        out.push_back(tmp.at(i));
    }

    return out;
}

uint32_t differenceSquared(const Point a, const Point b){
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

uint32_t orientation(const Point a, const Point b, const Point c){
    int det = (b.y - a.y)*(c.x - b.x) - (b.x - a.x)*(c.y - b.y);
    if (det == 0){
        return 0;
    }
    return (det > 0)? 1: 2;
}
