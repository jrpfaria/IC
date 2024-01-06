#include <iostream>
#include "yuv_reader.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    yuv_reader image = yuv_reader(argv[1]);
    Mat frame = image.get_frame(0);
    cv::imwrite("./frame0.png",frame);

    return 0;
}