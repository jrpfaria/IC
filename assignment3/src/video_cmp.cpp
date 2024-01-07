#include <iostream>
#include "video_cmp.h"
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

    // TODO: do something with video_cmp to test it

    return 0;
}