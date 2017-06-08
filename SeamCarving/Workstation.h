//
//  Workstation.h
//  SeamCarving
//
//  Created by bennyguo on 2017/6/7.
//  Copyright © 2017年 bennyguo. All rights reserved.
//

#ifndef Workstation_h
#define Workstation_h

#include <opencv2/core/core.hpp>
#include <vector>
#include "Seam.h"
#include "GradientOperator.h"

class Workstation
{
private:
    cv::Mat& img;
    int rows, cols, size;
    std::vector<Seam> seams;
    SimpleGradient gradientProcessor;
    void calculateSeams(int r, int c, cv::Mat& img)
    {
        this->img.copyTo(img);
        int** gradientMat = new int*[this->rows];
        int** dpMat = new int*[this->rows];
        int** pathMat = new int*[this->rows];
        for(int i = 0; i < this->rows; i ++)
        {
            gradientMat[i] = new int[this->cols];
            dpMat[i] = new int[this->cols];
            pathMat[i] = new int[this->cols];
        }
        this->gradientProcessor.gradient(img, gradientMat, this->rows, this->cols);
        this->seams.clear();
        int rows = this->rows, cols = this->cols;
        if(r == 0 && c > 0)
        {
            for(int k = 0; k < c; k ++)
            {
                Seam seam;
                seam.axis = 1;
                this->findVerticalSeam(gradientMat, dpMat, pathMat, seam, rows, cols);
                this->seams.push_back(seam);
                this->carve(img, gradientMat, seam, 1);
                cols -= 1;
                img = img(cv::Range::all(), cv::Range(0, cols));
                this->gradientProcessor.updateGradient(img, gradientMat, seam, rows, cols, 1);
            }
        }
        else if(c == 0 && r > 0)
        {
            for(int k = 0; k < r; k ++)
            {
                Seam seam;
                seam.axis = 0;
                this->findHorizontalSeam(gradientMat, dpMat, pathMat, seam, rows, cols);
                this->seams.push_back(seam);
                this->carve(img, gradientMat, seam, 0);
                rows -= 1;
                img = img(cv::Range(0, rows), cv::Range::all());
                this->gradientProcessor.updateGradient(img, gradientMat, seam, rows, cols, 0);
            }
        }
        else if(c > 0 && r > 0)
        {
            int cc = 0, rr = 0;
            for(int k = 0, tot = c + r; k < tot; k ++)
            {
                Seam seamH, seamV;
                seamH.axis = 0, seamV.axis = 1;
                int horizontalE = rr < r? this->findHorizontalSeam(gradientMat, dpMat, pathMat, seamH, rows, cols) : 1<<20;
                int verticalE = cc < c? this->findVerticalSeam(gradientMat, dpMat, pathMat, seamV, rows, cols) : 1<<20;
                if(horizontalE > verticalE)
                {
                    this->seams.push_back(seamV);
                    this->carve(img, gradientMat, seamV, 1);
                    cols -= 1;
                    cc += 1;
                    img = img(cv::Range(0, rows), cv::Range(0, cols));
                    this->gradientProcessor.updateGradient(img, gradientMat, seamV, rows, cols, 1);
                }
                else
                {
                    this->seams.push_back(seamH);
                    this->carve(img, gradientMat, seamH, 0);
                    rows -= 1;
                    rr += 1;
                    img = img(cv::Range(0, rows), cv::Range(0, cols));
                    this->gradientProcessor.updateGradient(img, gradientMat, seamH, rows, cols, 0);
                }
//                printf("Horizontal E: %d, Vertical E: %d\n", horizontalE, verticalE);
//                printf("cc: %d, rr: %d\n", cc, rr);
            }
        }
    }
    
    
    int findVerticalSeam(int** gradientMat, int** dpMat, int** pathMat, Seam& seam, int rows, int cols)
    {
        for(int i = 0; i < this->rows; i ++)
            for(int j = 0; j < this->cols; j ++)
            {
                dpMat[i][j] = 0;
                pathMat[i][j] = -1;
            }
        
        for(int i = 0; i < cols; i ++)
        {
            dpMat[0][i] = gradientMat[0][i];
            pathMat[0][i] = -1;
        }
        for(int i = 1; i < rows; i ++)
        {
            for(int j = 0; j < cols; j ++)
            {
                int minEnergy = 1<<20, prev = 0;
                for(int jj = j - 1; jj <= j + 1; jj ++)
                {
                    if(jj < 0 || jj >= cols)
                        continue;
                    int e = gradientMat[i - 1][jj] + gradientMat[i][j];
                    if(e < minEnergy)
                    {
                        minEnergy = e;
                        prev = jj;
                    }
                }
                dpMat[i][j] = minEnergy;
                pathMat[i][j] = prev;
            }
        }
        int minEnergy = 1<<20, bestEnd = 0;
        for(int i = 0; i < cols; i ++)
        {
            if(dpMat[rows - 1][i] < minEnergy)
            {
                minEnergy = dpMat[rows - 1][i];
                bestEnd = i;
            }
        }
        seam.clear();
        for(int i = rows - 1; i >= 0; i --)
        {
            seam.push_back(SeamPoint(i, bestEnd));
            bestEnd = pathMat[i][bestEnd];
        }
        reverse(seam.begin(), seam.end());
        return minEnergy;
    }
    
    int findHorizontalSeam(int** gradientMat, int** dpMat, int** pathMat, Seam& seam, int rows, int cols)
    {
        for(int i = 0; i < this->rows; i ++)
            for(int j = 0; j < this->cols; j ++)
            {
                dpMat[i][j] = 0;
                pathMat[i][j] = -1;
            }
        
        for(int i = 0; i < rows; i ++)
        {
            dpMat[i][0] = gradientMat[i][0];
            pathMat[i][0] = -1;
        }
        for(int i = 1; i < cols; i ++)
        {
            for(int j = 0; j < rows; j ++)
            {
                int minEnergy = 1<<20, prev = 0;
                for(int jj = j - 1; jj <= j + 1; jj ++)
                {
                    if(jj < 0 || jj >= rows)
                        continue;
                    int e = gradientMat[jj][i - 1] + gradientMat[j][i];
                    if(e < minEnergy)
                    {
                        minEnergy = e;
                        prev = jj;
                    }
                }
                dpMat[j][i] = minEnergy;
                pathMat[j][i] = prev;
            }
        }
        int minEnergy = 1<<20, bestEnd = 0;
        for(int i = 0; i < rows; i ++)
        {
            if(dpMat[i][cols - 1] < minEnergy)
            {
                minEnergy = dpMat[i][cols - 1];
                bestEnd = i;
            }
        }
        seam.clear();
        for(int i = cols - 1; i >= 0; i --)
        {
            seam.push_back(SeamPoint(bestEnd, i));
            bestEnd = pathMat[bestEnd][i];
        }
        reverse(seam.begin(), seam.end());
        return minEnergy;
    }
    
    void carve(cv::Mat& img, int** gradientMat, Seam& seam, int axis)
    {
        int rows = this->rows, cols = this->cols;
        if(axis == 1)
        {
            for(int i = 0, len = seam.size(); i < len; i ++)
            {
                int row = seam[i].row, col = seam[i].col;
                if(col > 0 && col < cols - 1)
                {
                    cv::Vec3b c0 = img.at<cv::Vec3b>(row, col),
                              c1 = img.at<cv::Vec3b>(row, col - 1),
                              c2 = img.at<cv::Vec3b>(row, col + 1);
                    img.at<cv::Vec3b>(row, col - 1) = cv::Vec3b((c0[0] + c1[0]) / 2, (c0[1] + c1[1]) / 2, (c0[2] + c1[2]) / 2);
                    img.at<cv::Vec3b>(row, col + 1) = cv::Vec3b((c0[0] + c2[0]) / 2, (c0[1] + c2[1]) / 2, (c0[2] + c2[2]) / 2);
                }
                for(int i = col + 1; i < cols; i ++)
                {
                    img.at<cv::Vec3b>(row, i - 1) = img.at<cv::Vec3b>(row, i);
                    gradientMat[row][i - 1] = gradientMat[row][i];
                }
            }
        }
        else if(axis == 0)
        {
            for(int i = 0, len = seam.size(); i < len; i ++)
            {
                int row = seam[i].row, col = seam[i].col;
                if(row > 0 && row < rows - 1)
                {
                    cv::Vec3b c0 = img.at<cv::Vec3b>(row, col),
                    c1 = img.at<cv::Vec3b>(row - 1, col),
                    c2 = img.at<cv::Vec3b>(row + 1, col);
                    img.at<cv::Vec3b>(row - 1, col) = cv::Vec3b((c0[0] + c1[0]) / 2, (c0[1] + c1[1]) / 2, (c0[2] + c1[2]) / 2);
                    img.at<cv::Vec3b>(row + 1, col) = cv::Vec3b((c0[0] + c2[0]) / 2, (c0[1] + c2[1]) / 2, (c0[2] + c2[2]) / 2);
                }
                for(int i = row + 1; i < rows; i ++)
                {
                    img.at<cv::Vec3b>(i - 1, col) = img.at<cv::Vec3b>(i, col);
                    gradientMat[i - 1][col] = gradientMat[i][col];
                }
            }
        }
    }
    
public:
    Workstation(cv::Mat& img): img(img)
    {
        this->rows = img.rows;
        this->cols = img.cols;
        this->size = this->rows * this->cols;
    }
    ~Workstation()
    {
        
    }
    void shrink(int r, int c, cv::Mat& result)
    {
        this->calculateSeams(r, c, result);
    }
    void drawSeams(cv::Mat& img)
    {
        this->img.copyTo(img);
        int** exist = new int*[this->rows];
        for(int i = 0; i < this->rows; i ++)
            exist[i] = new int[this->cols]{0};
        for(int i = 0, len = this->seams.size(); i < len; i ++)
        {
            Seam& seam = this->seams[i];
            if(seam.axis == 1)
            {
                for(int j = 0, seamLen = seam.size(); j < seamLen; j ++)
                {
                    int row = seam[j].row, col = seam[j].col;
                    int count = 0, pos = 0;
                    while(true)
                    {
                        if(exist[row][pos] != -1)
                        {
                            count += 1;
                            if(count > col)
                                break;
                        }
                        pos += 1;
                    }
                    exist[row][pos] = -1;
                    img.at<cv::Vec3b>(row, pos) = cv::Vec3b(0, 0, 255);
                    
                }
            }
            else if(seam.axis == 0)
            {
                for(int j = 0, seamLen = seam.size(); j < seamLen; j ++)
                {
                    int row = seam[j].row, col = seam[j].col;
                    int count = 0, pos = 0;
                    while(true)
                    {
                        if(exist[pos][col] != -1)
                        {
                            count += 1;
                            if(count > row)
                                break;
                        }
                        pos += 1;
                    }
                    exist[pos][col] = -1;
                    img.at<cv::Vec3b>(pos, col) = cv::Vec3b(0, 0, 255);
                    
                }
            }
        }
    }
};

#endif /* Workstation_h */