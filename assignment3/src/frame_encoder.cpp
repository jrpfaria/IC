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
    for (int h = 0; h < resolution[1]; h++) {
        for (int w = 0; w < resolution[0]; w++) {
            if (h==0 && w==0) pred[h*resolution[0]+w] = frame.at<uchar>(h,w);
            else if (h==0) pred[h*resolution[0]+w] = frame.at<uchar>(h,w) - (frame.at<uchar>(h,w-1)/3);
            else if (w==0) pred[h*resolution[0]+w] = frame.at<uchar>(h,w) - (frame.at<uchar>(h-1,w)/3);
            else pred[h*resolution[0]+w] = frame.at<uchar>(h,w) - ((frame.at<uchar>(h-1,w-1)+frame.at<uchar>(h-1,w)+frame.at<uchar>(h,w-1))/3);
        }
    }
    int m = Golomb::idealM(pred);
    BitStream bitstreamOutput {argv[2], 0};
    bitstreamOutput.write(image.get_resolution()[0],16);
    bitstreamOutput.write(image.get_resolution()[1],16);
    bitstreamOutput.write(image.get_aspect_ratio()[0],16);
    bitstreamOutput.write(image.get_aspect_ratio()[1],16);
    bitstreamOutput.write(image.get_frame_rate()[0],16);
    bitstreamOutput.write(image.get_frame_count(),16);
    bitstreamOutput.write(image.get_color_space(),16);
    bitstreamOutput.write(image.get_interlace(),16);
    bitstreamOutput.write(m,16);
    bitstreamOutput.write(method,16);
    Golomb g = Golomb(bitstreamOutput, m, method);
    for (auto p: pred) cout << p << endl, g.encode(p);
    bitstreamOutput.close();
    return 0;
}
