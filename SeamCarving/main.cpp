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
#include "Workstation.h"
using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    Mat img = imread("./8.jpeg");
    Workstation ws(img);
    Mat img_enlarged;
    ws.enlarge(400, 1, img_enlarged);
    namedWindow("test");
    imshow("test", img_enlarged);
    waitKey(0);
    return 0;
}
