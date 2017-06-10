//
//  ImageMarker.h
//  SeamCarving
//
//  Created by bennyguo on 2017/6/10.
//  Copyright © 2017年 bennyguo. All rights reserved.
//

#ifndef ImageMarker_h
#define ImageMarker_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ImageMarker
{
private:
    cv::Mat& img;
    cv::Mat& canvas;
    cv::Mat imgCopy;
    cv::Mat imgBlend;
    char windowName[10] = "Blend";
    cv::Scalar painterColor[2] = {cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255)};
    uchar canvasColor[2] = {1, 2};
    int mode = 0;
    static void onMouse(int event, int x, int y, int flags, void* obj)
    {
        ImageMarker* marker = reinterpret_cast<ImageMarker*>(obj);
        if(event == CV_EVENT_LBUTTONDOWN || (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)))
        {
            cv::circle(marker->imgCopy, cv::Point(x, y), 20, marker->painterColor[marker->mode], -1);
            cv::circle(marker->canvas, cv::Point(x, y), 20, marker->canvasColor[marker->mode], -1);
            cv::addWeighted(marker->img, 0.6, marker->imgCopy, 0.4, 0, marker->imgBlend);
            cv::imshow(marker->windowName, marker->imgBlend);
        }
        if(event == CV_EVENT_RBUTTONDOWN)
        {
            if(marker->mode == 1)
            {
                marker->mode = 0;
                printf("Paint the area you want to remove <RIGHT click to switch>\n");
            }
            else
            {
                marker->mode = 1;
                printf("Paint the area you want to protect <RIGHT click to switch>\n");
            }
        }
    }
public:
    ImageMarker(cv::Mat& img, cv::Mat& canvas, uchar remove = 1, uchar protect = 2): img(img), canvas(canvas)
    {
        this->img.copyTo(this->imgCopy);
        this->canvas = cv::Mat::zeros(this->img.size(), CV_8UC1);
        this->canvasColor[0] = remove;
        this->canvasColor[1] = protect;
    }
    ~ImageMarker()
    {

    }
    void mark()
    {
        printf("Paint the area you want to reomve <RIGHT click to switch>\n");
        cv::namedWindow(this->windowName);
        cv::setMouseCallback(this->windowName, this->onMouse, this);
        cv::imshow(this->windowName, this->imgCopy);
        cv::waitKey(0);
    }
};

#endif /* ImageMarker_h */
