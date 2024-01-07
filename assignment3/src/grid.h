#ifndef grid_h
#define grid_h

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Grid {
    private:
        Mat &frame;
        int block_heigth;
        int block_width;
    
    public:
        Grid(Mat& _frame, int block_heigth, int block_width): frame(_frame) {
            this->block_heigth = block_heigth;
            this->block_width = block_width;
        }

        int heigth() {
            return ceil(frame.rows/block_heigth);
        }

        int width() {
            return ceil(frame.cols/block_width);
        }

        Mat block(int x, int y) {
            int local_heigth = block_heigth;
            int local_width = block_width;
            if ((x+1)*block_width>frame.cols) local_width = frame.cols - x*block_width;
            if ((y+1)*block_heigth>frame.rows) local_heigth = frame.rows - y*block_heigth;
            Rect rect = Rect(local_width,local_heigth,x*block_width,y*block_heigth);
            Mat block = frame(rect).clone();
            return block;
        }

        void set_block(Mat block, int x, int y) {
            for (int h = 0; h < block.rows; h++) {
                for (int w = 0; w < block.cols; w++) {
                    frame.at<uchar>((y*block_heigth)+h, (x*block_width)+w) = block.at<uchar>(h,w);
                }
            }
        }
};

#endif
