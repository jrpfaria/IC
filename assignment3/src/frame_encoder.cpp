#include <iostream>
#include "yuv_reader.h"
#include "golomb.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{   
    int method = 0;
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <input> <output>" << std::endl;
        return 1;
    }

    yuv_reader image = yuv_reader(argv[1]);
    int* resolution = image.get_resolution();
    vector<int> pred(resolution[0]*resolution[1]);
    Mat frame = image.get_frame(0);
    pred[0] = frame.at<uchar>(0,0);
    for (int h = 1; h < resolution[1]; h++) {
        pred[h*resolution[0]] = frame.at<uchar>(h,0) - (frame.at<uchar>(h-1,0)/3);
    }
    for (int w = 1; w < resolution[0]; w++) {
        pred[w*resolution[1]] = frame.at<uchar>(0,w) - (frame.at<uchar>(0,w-1)/3);
    }
    for (int h = 2; h < resolution[1]; h++) {
        for (int w = 2; w < resolution[0]; w++) {
            pred[h*resolution[0]+w] = frame.at<uchar>(h,w) - ((frame.at<uchar>(h-1,w-1)+frame.at<uchar>(h-1,w)+frame.at<uchar>(h,w-1))/3);
        }
    }
    BitStream bitstreamOutput {argv[2], 0};
      //  std::string interlace;
        //std::string color_space;
    bitstreamOutput.write(image.get_resolution()[0],16);
    bitstreamOutput.write(image.get_resolution()[1],16);
    bitstreamOutput.write(image.get_aspect_ratio()[0],16);
    bitstreamOutput.write(image.get_aspect_ratio()[1],16);
    bitstreamOutput.write(image.get_frame_rate()[0],16);
    bitstreamOutput.write(image.get_frame_count(),16);
    bitstreamOutput.write(image.get_resolution()[0],16);
    bitstreamOutput.write(0,16);
    bitstreamOutput.write(0,16);
    int m = Golomb::idealM(pred);
    Golomb g = Golomb(bitstreamOutput, m, method);
    for (auto p: pred) g.encode(p);
    return 0;
}
