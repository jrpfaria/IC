#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace imageFilter
{
    static Mat *getFrame()
    {
        // convert yuv into various frames
    }

    // Set desired block dimension
    #define blockWidth  192 // random for now
    #define blockHeight 108 // random for now

    // \brief returns a Mat object with the block at (block_x, block_y)
    static Mat getBlock(Mat image, int block_x, int block_y)
    {
        Mat block = Mat::zeros(blockHeight, blockWidth, CV_8UC1);

        int x_lower_bound = blockWidth * block_x;
        int x_upper_bound = blockWidth * (block_x + 1);

        int y_lower_bound = blockHeight * block_y;
        int y_upper_bound = blockHeight * (block_y + 1);

        for (int i = y_lower_bound; i < y_upper_bound; i++)
            for (int j = x_lower_bound; j < x_upper_bound; j++)
                block.at<uchar>(i, j) = image.at<uchar>(i, j);

        return block;
    }
};

#endif