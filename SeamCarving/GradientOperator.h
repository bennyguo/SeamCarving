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

class GradientOperator
{
public:
    GradientOperator()
    {
        
    }
    ~GradientOperator()
    {
        
    }
    void gradient(cv::Mat& img, int** gradientMat, int rows, int cols)
    {
        cv::Mat imgWithBorder;
        cv::copyMakeBorder(img, imgWithBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);
        for(int i = 1; i <= rows; i ++)
            for(int j = 1; j <= cols; j ++)
            {
                gradientMat[i - 1][j - 1] = calcGradient(imgWithBorder, i, j);
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
                    gradientMat[row - 1][col - 1] = calcGradient(imgWithBorder, row, col);
                }
                if(col >= 2)
                {
                    col = col - 1;
                    gradientMat[row - 1][col - 1] = calcGradient(imgWithBorder, row, col);
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
                    gradientMat[row - 1][col - 1] = calcGradient(imgWithBorder, row, col);
                }
                if(row >= 2)
                {
                    row = row - 1;
                    gradientMat[row - 1][col - 1] = calcGradient(imgWithBorder, row, col);
                }
            }
        }
    }
    int RGBGradientSq(cv::Vec3b& c1, cv::Vec3b& c2)
    {
        int db = c1[0] - c2[0], dg = c1[1] - c2[1], dr = c1[2] - c2[2];
        return db * db + dg * dg + dr * dr;
    }
    virtual inline int calcGradient(cv::Mat& img, int row, int col) = 0;
};

class EuclidGradient: public GradientOperator
{
public:
    inline int calcGradient(cv::Mat &img, int row, int col)
    {
        return (int)sqrt(this->RGBGradientSq(img.at<cv::Vec3b>(row - 1, col), img.at<cv::Vec3b>(row + 1, col))
                       + this->RGBGradientSq(img.at<cv::Vec3b>(row, col - 1), img.at<cv::Vec3b>(row, col + 1)));
    }
};

class AbsGradient: public GradientOperator
{
public:
    inline int calcGradient(cv::Mat &img, int row, int col)
    {
        return (int)sqrt(this->RGBGradientSq(img.at<cv::Vec3b>(row - 1, col), img.at<cv::Vec3b>(row + 1, col)))
             + (int)sqrt(this->RGBGradientSq(img.at<cv::Vec3b>(row, col - 1), img.at<cv::Vec3b>(row, col + 1)));
    }
};

class SobelGradient: public GradientOperator
{
public:
    inline int calcGradient(cv::Mat &img, int row, int col)
    {
        cv::Vec3b c00 = img.at<cv::Vec3b>(row - 1, col - 1), c01 = img.at<cv::Vec3b>(row - 1, col), c02 = img.at<cv::Vec3b>(row - 1, col  + 1),
            c10 = img.at<cv::Vec3b>(row, col - 1), c11 = img.at<cv::Vec3b>(row, col), c12 = img.at<cv::Vec3b>(row, col + 1),
            c20 = img.at<cv::Vec3b>(row + 1, col - 1), c21 = img.at<cv::Vec3b>(row + 1, col), c22 = img.at<cv::Vec3b>(row + 1, col + 1);
        int gxb = c00[0] - c02[0] + 2 * (c10[0] - c12[0]) + c20[0] - c22[0],
            gxg = c00[1] - c02[1] + 2 * (c10[1] - c12[1]) + c20[1] - c22[1],
            gxr = c00[2] - c02[2] + 2 * (c10[2] - c12[2]) + c20[2] - c22[2];
        int gyb = c00[0] - c20[0] + 2 * (c01[0] - c21[0]) + c02[0] - c22[0],
            gyg = c00[1] - c20[1] + 2 * (c01[1] - c21[1]) + c02[1] - c22[1],
            gyr = c00[2] - c20[2] + 2 * (c01[2] - c21[2]) + c02[2] - c22[2];
        return (int)sqrt(gxb * gxb + gxg * gxg + gxr * gxr + gyb * gyb + gyg * gyg + gyr * gyr);
    }
};

#endif /* GradientOperator_h */
