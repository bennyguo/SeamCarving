//
//  main.cpp
//  SeamCarving
//
//  Created by bennyguo on 2017/3/28.
//  Copyright © 2017年 bennyguo. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Workstation.h"
#include "GradientOperator.h"
using namespace cv;
using namespace std;


int main(int argc, const char * argv[]) {
    GradientOperator* simple = new EuclidGradient();
    Mat img = imread("./5.jpg");
    Workstation ws(img, simple);
    Mat result;
//    ws.shrink(0, 200, result);
//    ws.enlarge(200, 1, result);
    ws.remove(result);
//    ws.drawSeams(result);
    namedWindow("result");
    imshow("result", result);
    imwrite("test.jpg", result);
    waitKey(0);
    return 0;
}
