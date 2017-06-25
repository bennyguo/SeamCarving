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
    GradientOperator* grad = new AbsGradient();
    Mat img = imread("1.jpeg");
    Workstation ws(img, grad);
    Mat result, seams;
    ws.shrink(0, img.cols / 4, result);
    ws.drawSeams(seams);
    imwrite("shrink.jpg", result);
    imwrite("seams.jpg", seams);
    return 0;
}
