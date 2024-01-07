#include <iostream>
#include "yuv_reader.h"
#include "golomb.h"
#include "grid.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<int> intra_prediction(Mat frame) {
    vector<int> pred(frame.cols*frame.rows);
    for (int h = 0; h < frame.rows; h++) {
        for (int w = 0; w < frame.cols; w++) {
            if (h==0 && w==0) pred[h*frame.cols+w] = frame.at<uchar>(h,w);
            else if (h==0) pred[h*frame.cols+w] = frame.at<uchar>(h,w) - (frame.at<uchar>(h,w-1)/3);
            else if (w==0) pred[h*frame.cols+w] = frame.at<uchar>(h,w) - (frame.at<uchar>(h-1,w)/3);
            else pred[h*frame.cols+w] = frame.at<uchar>(h,w) - ((frame.at<uchar>(h-1,w-1)+frame.at<uchar>(h-1,w)+frame.at<uchar>(h,w-1))/3);
        }
    }
    return pred;
}

vector<int> inter_prediction(Mat frame, Mat framePrevious) {
    vector<int> pred(frame.cols*frame.rows);
    for (int h = 0; h < frame.rows; h++) {
        for (int w = 0; w < frame.cols; w++) {
            pred[h*frame.cols+w] = frame.at<uchar>(h,w) - framePrevious.at<uchar>(h,w);
        }
    }
    return pred;
}

int main(int argc, char *argv[])
{   
    int method = 0;
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " -h <block heigth> -w <block width> -p <periodicity> <input> <output>" << std::endl;
        return 1;
    }
    bool inter = false;
    int heigth = 1;
    int width = 1;
    int periodicity = 1;
    for (int n = 1; n < argc; n++) {
        if(string(argv[n]) == "-h") {
            inter = true;
            heigth = stoi(argv[n+1]);
        }
        if(string(argv[n]) == "-w") {
            inter = true;
            width = stoi(argv[n+1]);
        }
        if(string(argv[n]) == "-p") {
            inter = true;
            periodicity = stoi(argv[n+1]);
        }
    }

    yuv_reader image = yuv_reader(argv[argc-2]);
    int* resolution = image.get_resolution();
    int frame_count = image.get_frame_count();
    BitStream bitstreamOutput {argv[argc-1], 0};
    bitstreamOutput.write(resolution[0],16);
    bitstreamOutput.write(resolution[1],16);
    bitstreamOutput.write(image.get_aspect_ratio()[0],16);
    bitstreamOutput.write(image.get_aspect_ratio()[1],16);
    bitstreamOutput.write(image.get_frame_rate()[0],16);
    bitstreamOutput.write(image.get_frame_rate()[1],16);
    bitstreamOutput.write(image.get_frame_count(),16);
    bitstreamOutput.write(ColorSpace::Cmono,16);
    bitstreamOutput.write(image.get_interlace(),16);
    bitstreamOutput.write(method);
    bitstreamOutput.write(inter);
    if (inter) {
        bitstreamOutput.write(heigth, 16);
        bitstreamOutput.write(width, 16);
        bitstreamOutput.write(periodicity, 16);
    }

    Mat frame;
    Mat framePrevious;
    for (int i = 0; i < frame_count; i++) {
        vector<int> pred(resolution[0]*resolution[1]);
        frame = image.get_frame(i);
        if (inter && i%periodicity!=0) {
            Grid grid { frame, heigth, width };
            Grid gridPrevious { framePrevious, heigth, width };
            for (int y = 0; y < grid.heigth(); y++) {
                for (int x = 0; x < grid.width(); x++) {
                    Mat block = grid.block(x, y);
                    int best_size = INT32_MAX;
                    int bestX = -1;
                    int bestY = -1;
                    vector<int> local_pred(heigth*width);
                    vector<int> best_pred = intra_prediction(block);
                    {
                        int m = Golomb::idealM(local_pred);
                        Golomb g = Golomb(bitstreamOutput, m, method);
                        best_size = g.getEncodedLength(local_pred);
                    }
                    for (int xP = 0; xP < grid.width(); xP++) {
                        for (int yP = 0; yP < grid.heigth(); yP++) {
                            try {
                                local_pred = inter_prediction(block, gridPrevious.block(xP, yP));
                                int m = Golomb::idealM(local_pred);
                                Golomb g = Golomb(bitstreamOutput, m, method);
                                int local_size = g.getEncodedLength(local_pred);
                                if (local_size<best_size) {
                                    best_pred = local_pred;
                                    best_size = local_size;
                                    bestX = xP;
                                    bestY = yP;
                                }
                            }
                            catch (const exception &) {}
                        }
                    }
                    int m = Golomb::idealM(best_pred);
                    Golomb g = Golomb(bitstreamOutput, m, method);
                    bitstreamOutput.write(m, 16);
                    if (bestX==-1 || bestY==-1) bitstreamOutput.write(0);
                    else {
                        bitstreamOutput.write(1);
                        bitstreamOutput.write(123,16);
                        bitstreamOutput.write(bestX, 16);
                        bitstreamOutput.write(bestY, 16);
                    }
                    for (auto p: best_pred) g.encode(p);
                }
            }
        }
        else {
            pred = intra_prediction(frame);
            int m = Golomb::idealM(pred);
            Golomb g = Golomb(bitstreamOutput, m, method);
            bitstreamOutput.write(m,16);
            for (auto p: pred) g.encode(p);
        }
        framePrevious = frame.clone();
    }
    
    bitstreamOutput.close();
    return 0;
}
