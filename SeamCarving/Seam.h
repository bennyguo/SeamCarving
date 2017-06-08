//
//  Seam.h
//  SeamCarving
//
//  Created by bennyguo on 2017/6/7.
//  Copyright © 2017年 bennyguo. All rights reserved.
//

#ifndef Seam_h
#define Seam_h

#include <vector>
class SeamPoint
{
public:
    int row;
    int col;
    SeamPoint(int row, int col)
    {
        this->row = row;
        this->col = col;
    }
};
//typedef std::vector<SeamPoint> Seam;
class Seam: public std::vector<SeamPoint>
{
public:
    int energy;
    int axis;
};


#endif /* Seam_h */
