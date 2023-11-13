#ifndef PPMEFFECTS_H
#define PPMEFFECTS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace ppmeffects {
    static Mat extract(Mat image, char color) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);
        int ch = 0;
        switch(color){
            case 'b':
                ch = 0;
                break;
            case 'g':
                ch = 1;
                break;
            case 'r':
                ch = 2;
                break;
            default:
                break;
        }
        for (int i = 0; i < image.rows; i++) 
            for (int j = 0; j < image.cols; j++) 
                result.at<uchar>(i,j) = image.at<Vec3b>(i,j)[ch];
        return result;
    }

    static Mat negative(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++){
                result.at<Vec3b>(i,j)[0] = 0xFF - image.at<Vec3b>(i,j)[0];
                result.at<Vec3b>(i,j)[1] = 0xFF - image.at<Vec3b>(i,j)[1];
                result.at<Vec3b>(i,j)[2] = 0xFF - image.at<Vec3b>(i,j)[2];
            }
        return result;
    }

    static Mat mirrorHorizontally(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++){
                result.at<Vec3b>(i,j)[0] = image.at<Vec3b>(i,image.cols - j)[0];
                result.at<Vec3b>(i,j)[1] = image.at<Vec3b>(i,image.cols - j)[1];
                result.at<Vec3b>(i,j)[2] = image.at<Vec3b>(i,image.cols - j)[2];
            }
        return result;
    }

    static Mat mirrorVertically(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++){
                result.at<Vec3b>(i,j)[0] = image.at<Vec3b>(image.rows - i,j)[0];
                result.at<Vec3b>(i,j)[1] = image.at<Vec3b>(image.rows - i,j)[1];
                result.at<Vec3b>(i,j)[2] = image.at<Vec3b>(image.rows - i,j)[2];
            }
        return result;
    }

    static Mat mirror(Mat image, unsigned char orientation) {
        return orientation == 'v' ? mirrorVertically(image) : mirrorHorizontally(image);
    }

    static Mat rotate(Mat image, int angle) {
        Mat result;
        int width = image.cols;
        int height = image.rows;

        angle = (angle % 360) >= 0 ? (angle % 360) : (angle % 360) + 360;
        

        if ((angle == 90) || (angle == 270)) result = Mat::zeros(width, height, CV_8UC3);
        else if (angle == 180) result = Mat::zeros(height, width, CV_8UC3);
        else result = image.clone();

        switch (angle % 360){
            case 90:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++){
                        result.at<Vec3b>(width - 1 - j, i)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(width - 1 - j, i)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(width - 1 - j, i)[2] = image.at<Vec3b>(i, j)[2];
                    }
                break;
            case 180:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++){
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[2] = image.at<Vec3b>(i, j)[2];
                    }
                break;
            case 270:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++){
                        result.at<Vec3b>(j, height - 1 - i)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(j, height - 1 - i)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(j, height - 1 - i)[2] = image.at<Vec3b>(i, j)[2];
                    }
                break;
            default:
                break;
        }

        return result;
    }

    static Mat changeIntensity(Mat image, float intensity) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++){
                result.at<Vec3b>(i,j)[0] = image.at<Vec3b>(i,j)[0] * intensity;
                result.at<Vec3b>(i,j)[1] = image.at<Vec3b>(i,j)[1] * intensity;
                result.at<Vec3b>(i,j)[2] = image.at<Vec3b>(i,j)[2] * intensity;
            }
        return result;
    }
};

#endif
