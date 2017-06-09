//
//  GradientOperator.h
//  SeamCarving
//
//  Created by bennyguo on 2017/6/7.
//  Copyright © 2017年 bennyguo. All rights reserved.
//

#ifndef GradientOperator_h
#define GradientOperator_h

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Seam.h"

class SimpleGradient
{
private:
    int RGBGradientSq(cv::Vec3b& c1, cv::Vec3b& c2)
    {
        int db = c1[0] - c2[0], dg = c1[1] - c2[1], dr = c1[2] - c2[2];
        return db * db + dg * dg + dr * dr;
    }
    
public:
    SimpleGradient()
    {
        
    }
    
    ~SimpleGradient()
    {
        
    }
    
    void gradient(cv::Mat& img, int** gradientMat, int rows, int cols)
    {
        cv::Mat imgWithBorder;
        cv::copyMakeBorder(img, imgWithBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);
        for(int i = 1; i <= rows; i ++)
            for(int j = 1; j <= cols; j ++)
            {
                int gradSq = this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(i - 1, j), imgWithBorder.at<cv::Vec3b>(i + 1, j))
                           + this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(i, j - 1), imgWithBorder.at<cv::Vec3b>(i, j + 1));
                gradientMat[i - 1][j - 1] = (int)sqrt(gradSq);
            }
    }
    
    void updateGradient(cv::Mat& img, int** gradientMat, Seam& seam, int rows, int cols, int axis)
    {
        cv::Mat imgWithBorder;
        cv::copyMakeBorder(img, imgWithBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);
        if(axis == 1)
        {
            for(int i = 0, len = seam.size(); i < len ; i ++)
            {
                int row = seam[i].row + 1, col = seam[i].col + 1;
                if(col <= cols)
                {
                    int gradSq = this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row - 1, col), imgWithBorder.at<cv::Vec3b>(row + 1, col))
                               + this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row, col - 1), imgWithBorder.at<cv::Vec3b>(row, col + 1));
                    gradientMat[row - 1][col - 1] = (int)sqrt(gradSq);
                }
                if(col >= 2)
                {
                    col = col - 1;
                    int gradSq = this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row - 1, col), imgWithBorder.at<cv::Vec3b>(row + 1, col))
                               + this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row, col - 1), imgWithBorder.at<cv::Vec3b>(row, col + 1));
                    gradientMat[row - 1][col - 1] = (int)sqrt(gradSq);
                }
            }
        }
        else if(axis == 0)
        {
            for(int i = 0, len = seam.size(); i < len ; i ++)
            {
                int row = seam[i].row + 1, col = seam[i].col + 1;
                if(row <= rows)
                {
                    int gradSq = this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row - 1, col), imgWithBorder.at<cv::Vec3b>(row + 1, col))
                               + this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row, col - 1), imgWithBorder.at<cv::Vec3b>(row, col + 1));
                    gradientMat[row - 1][col - 1] = (int)sqrt(gradSq);
                }
                if(row >= 2)
                {
                    row = row - 1;
                    int gradSq = this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row - 1, col), imgWithBorder.at<cv::Vec3b>(row + 1, col))
                               + this->RGBGradientSq(imgWithBorder.at<cv::Vec3b>(row, col - 1), imgWithBorder.at<cv::Vec3b>(row, col + 1));
                    gradientMat[row - 1][col - 1] = (int)sqrt(gradSq);
                }
            }
        }
    }
};

#endif /* GradientOperator_h */
