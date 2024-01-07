#include <iostream>
#include "yuv_writer.h"
#include "golomb.h"
#include "grid.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat intra_prediction(Golomb g, int* resolution) {
    Mat frame = Mat::zeros(resolution[1], resolution[0], CV_8UC1);
    for (int h = 0; h < resolution[1]; h++) {
        for (int w = 0; w < resolution[0]; w++) {
            int p = g.decode();
            if (h==0 && w==0) frame.at<uchar>(h,w) = p;
            else if (h==0) frame.at<uchar>(h,w) = p + (frame.at<uchar>(h,w-1)/3);
            else if (w==0) frame.at<uchar>(h,w) = p + (frame.at<uchar>(h-1,w)/3);
            else frame.at<uchar>(h,w) = p + ((frame.at<uchar>(h-1,w-1)+frame.at<uchar>(h-1,w)+frame.at<uchar>(h,w-1))/3);
        }
    }
    return frame;
}

Mat inter_prediction(Golomb g, Mat blockPrevious) {
    Mat frame = Mat::zeros(blockPrevious.rows, blockPrevious.cols, CV_8UC1);
    for (int h = 0; h < blockPrevious.rows; h++) {
        for (int w = 0; w < blockPrevious.cols; w++) {
            int p = g.decode();
            frame.at<uchar>(h,w) = p + blockPrevious.at<uchar>(h,w);
        }
    }
    return frame;
}

int main(int argc, char *argv[])
{   
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <input> <output>" << std::endl;
        return 1;
    }

    int heigth = 1;
    int width = 1;
    int periodicity = 1;

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
    int frame_count = bitstreamInput.readInt(16);
    ColorSpace color_space = ColorSpace(bitstreamInput.readInt(16));
    Interlace interlace = Interlace(bitstreamInput.readInt(16));
    int method = bitstreamInput.read();
    bool inter = bitstreamInput.read();
    if (inter) {
        heigth = bitstreamInput.readInt(16);
        width = bitstreamInput.readInt(16);
        periodicity = bitstreamInput.readInt(16);
    }

    yuv_writer image = yuv_writer(argv[2]);
    image.set_resolution(resolution);
    image.set_aspect_ratio(aspect_ratio);
    image.set_frame_rate(frame_rate);
    image.set_color_space(color_space);
    image.set_interlace(interlace);
    image.write_header();

    Mat frame;
    Mat framePrevious;
    for (int i = 0; i < frame_count; i++) {
        std::cout << "frame: " << i << std::endl;
        frame = Mat::zeros(resolution[1], resolution[0], CV_8UC1);
        if (inter && i%periodicity!=0) {
            Grid grid { frame, heigth, width };
            Grid gridPrevious { framePrevious, heigth, width };
            for (int y = 0; y < grid.heigth(); y++) {
                for (int x = 0; x < grid.width(); x++) {
                    int m = bitstreamInput.readInt(16);
                    Golomb g = Golomb(bitstreamInput, m, method);
                    bool type = bitstreamInput.read();
                    if (type) {
                        std::cout << "check: " << bitstreamInput.readInt(16) << std::endl;
                        int xP = bitstreamInput.readInt(16);
                        int yP = bitstreamInput.readInt(16);
                        std::cout << "x:" << xP << " y:" << yP << std::endl;
                        Mat blockPrevious = gridPrevious.block(xP, yP);
                        grid.set_block(inter_prediction(g, blockPrevious), x, y);
                    }
                    else {
                        grid.set_block(intra_prediction(g, resolution), x, y);
                    }
                }
            }
        }
        else {
            int m = bitstreamInput.readInt(16);
            Golomb g = Golomb(bitstreamInput, m, method);
            std::cout << "intra" << std::endl;
            frame = intra_prediction(g, resolution);
            std::cout << "done" << std::endl;
        }
        std::cout << "write" << std::endl;
        image.write_frame(frame);
        framePrevious = frame.clone();
        std::cout << "done" << std::endl;
    }

    image.close();
    return 0;
}
