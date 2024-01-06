#include <iostream>
#include "yuv_reader.h"
#include "golomb.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{   
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <input> <output>" << std::endl;
        return 1;
    }

    int resolution[2];
    int aspect_ratio[2];
    int frame_rate[2];
    BitStream bitstreamInput {argv[1], 1};
    resolution[0] = bitstreamInput.readInt(16);
    resolution[1] = bitstreamInput.readInt(16);
    aspect_ratio[0] = bitstreamInput.readInt(16);
    aspect_ratio[1] = bitstreamInput.readInt(16);
    frame_rate[0] = bitstreamInput.readInt(16);
    frame_rate[1] = bitstreamInput.readInt(16);
    ColorSpace color_space = ColorSpace(bitstreamInput.readInt(16));
    Interlace interlace = Interlace(bitstreamInput.readInt(16));
    int m = bitstreamInput.readInt(16);
    int method = bitstreamInput.readInt(16);
    Golomb g = Golomb(bitstreamInput, m, method);

    Mat frame = Mat::zeros(resolution[1], resolution[0], CV_8UC1);
    for (int h = 0; h < resolution[1]; h++) {
        for (int w = 0; w < resolution[0]; w++) {
            int p = g.decode();
            std::cout << p << endl;
            if (h==0 && w==0) frame.at<uchar>(h,w) = p;
            else if (h==0) frame.at<uchar>(h,w) = p + (frame.at<uchar>(h,w-1)/3);
            else if (w==0) frame.at<uchar>(h,w) = p + (frame.at<uchar>(h-1,w)/3);
            else frame.at<uchar>(h,w) = p + ((frame.at<uchar>(h-1,w-1)+frame.at<uchar>(h-1,w)+frame.at<uchar>(h,w-1))/3);
        }
    }
    cv::imwrite("./frame1.png",frame);
    
    return 0;
}
