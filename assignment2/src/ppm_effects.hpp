#ifndef PPMEFFECTS_H
#define PPMEFFECTS_H

#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace ppmeffects {
    static Mat extract(Mat image, char color) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);
        int ch = 0;
        switch(color){
            case 'b': ch = 0; break;
            case 'g': ch = 1; break;
            case 'r': ch = 2; break;
            default: break;
        }
        for (int i = 0; i < image.rows; i++) 
            for (int j = 0; j < image.cols; j++) 
                result.at<uchar>(i,j) = image.at<Vec3b>(i,j)[ch];
        return result;
    }

    static Mat negative(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++)
                for (int k = 0; k < 3; k++)
                    result.at<Vec3b>(i,j)[k] = 0xFF - image.at<Vec3b>(i,j)[k];
        return result;
    }

    static Mat mirrorHorizontally(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++)
                for (int k = 0; k < 3; k++)
                    result.at<Vec3b>(i,j)[k] = image.at<Vec3b>(i,image.cols - j)[k];
        return result;
    }

    static Mat mirrorVertically(Mat image) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++)
                for (int k = 0; k < 3; k++)
                    result.at<Vec3b>(i,j)[k] = image.at<Vec3b>(image.rows - i,j)[k];
        return result;
    }

    static Mat mirror(Mat image, unsigned char orientation) {
        return orientation == 'v' ? mirrorVertically(image) : mirrorHorizontally(image);
    }

    static Mat rotate(Mat image, int angle) {
        Mat result;
        int width = image.cols;
        int height = image.rows;

        angle = angle % 360; 
        if (angle < 0) angle += 360;

        if ((angle == 90) || (angle == 270)) result = Mat::zeros(width, height, CV_8UC3);
        else if (angle == 180) result = Mat::zeros(height, width, CV_8UC3);
        else return image;

        switch (angle){
            case 90:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++)
                        for (int k = 0; k < 3; k++)
                            result.at<Vec3b>(j, height - 1 - i)[k] = image.at<Vec3b>(i, j)[k];
                break;
            case 180:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++)
                        for (int k = 0; k < 3; k++)
                            result.at<Vec3b>(height - 1 - i, width - 1 - j)[k] = image.at<Vec3b>(i, j)[k];
                break;
            case 270:
                for (int i = 0; i < height; i++) 
                    for (int j = 0; j < width; j++)
                        for (int k = 0; k < 3; k++)
                            result.at<Vec3b>(width - 1 - j, i)[k] = image.at<Vec3b>(i, j)[k];
                break;
            default:
                break;
        }

        return result;
    }

    static int adaptIntensity(int value, float intensity) {
        int result = 0;
        // This formula adds a percentage of the complementary of the value to the original value
        // This way we have a progression of the intensity that resembles a logarithmic curve
        if (intensity > 1) result = (intensity - 1) * (255 - value) + value;
        // If we want to decrease the intensity, we subtract a percentage of the value to the original value
        // Effectively achieving an inverse logarithmic curve
        else result = value - (1 - intensity) * value;
        result = std::max(0, std::min(result, 255)); // clamp result to 0-255
        return result;
    }

    static Mat changeIntensity(Mat image, float intensity) {
        Mat result = Mat::zeros(image.rows, image.cols, CV_8UC3);
        for (int i = 0; i < image.rows; i++)
            for (int j = 0; j < image.cols; j++)
                for (int k = 0; k < 3; k++)
                    result.at<Vec3b>(i,j)[k] = adaptIntensity(image.at<Vec3b>(i,j)[k], intensity);
        return result;
    }
};

#endif
