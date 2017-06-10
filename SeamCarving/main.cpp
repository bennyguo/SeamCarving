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
    GradientOperator* simple = new SimpleGradient();
    Mat img = imread("./7.jpeg");
    Workstation ws(img, simple);
    Mat removed;
    ws.remove(removed);
    namedWindow("test");
    imshow("test", removed);
    waitKey(0);
    return 0;
}
