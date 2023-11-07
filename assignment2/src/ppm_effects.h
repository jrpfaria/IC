#ifndef PPMEFFECTS_H
#define PPMEFFECTS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PPMEffects {
    private:
        int color; // 0 = blue, 1 = green, 2 = red 

    public:
        PPMEffects(char color) {
            switch (color)
            {
                case 'b':
                    this->color = 0;
                    break;
                case 'g':
                    this->color = 1;
                    break;
                case 'r':
                    this->color = 2;
                    break;
                default:
                    this->color = 0; // Blue as default
                    break;
            }
        }

        Mat extract(Mat image) {
            Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);
            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    result.at<uchar>(i,j) = image.at<Vec3b>(i,j)[this->color];
                }
            }
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

        static Mat mirror(Mat image, unsigned char orientation) {
            return orientation == 'v' ? mirrorVertically(image) : mirrorHorizontally(image);
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

        static Mat rotate(Mat image, int angle) {
            Mat result;
            int width = image.cols;
            int height = image.rows;

            if ((angle % 360 == 90) || (angle % 360 == 270)) {
                result = Mat::zeros(width, height, CV_8UC3);
            } else if (angle % 360 == 180) {
                result = Mat::zeros(height, width, CV_8UC3);
            } else {
                result = image.clone();
            }

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {

                    if (angle % 360 == 90) {
                        result.at<Vec3b>(width - 1 - j, i)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(width - 1 - j, i)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(width - 1 - j, i)[2] = image.at<Vec3b>(i, j)[2];

                    } else if (angle % 360 == 180) {
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(height - 1 - i, width - 1 - j)[2] = image.at<Vec3b>(i, j)[2];
                    
                    } else if (angle % 360 == 270) {
                        result.at<Vec3b>(j, height - 1 - i)[0] = image.at<Vec3b>(i, j)[0];
                        result.at<Vec3b>(j, height - 1 - i)[1] = image.at<Vec3b>(i, j)[1];
                        result.at<Vec3b>(j, height - 1 - i)[2] = image.at<Vec3b>(i, j)[2];

                    } else {
                        continue;
                    }
                }
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
