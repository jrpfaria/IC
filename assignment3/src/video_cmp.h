#ifndef video_cmp_h
#define video_cmp_h

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class video_cmp
{
    private:
        double peak_signal_to_noise_ratio;

    public:
        video_cmp(Mat original_frame, Mat new_frame, int max_pixel_value = 255)
        {
            this->peak_signal_to_noise_ratio = psnr(original_frame, new_frame, max_pixel_value);
        }

        double psnr(Mat original_frame, Mat new_frame, int max_pixel_value = 255)
        {
            double mean_squared_error = mse(original_frame, new_frame);
            double max_pixel_value_squared = pow(max_pixel_value, 2);
            double peak_signal_to_noise_ratio = 10 * log10((max_pixel_value_squared) / mean_squared_error);

            return peak_signal_to_noise_ratio;
        }

        double mse(Mat original_frame, Mat new_frame)
        {
            double mean_squared_error = 0.0;
            int height = original_frame.rows;
            int width = original_frame.cols;

            for(int i = 0; i < height; i++)
                for(int j = 0; j < width; j++)
                    mean_squared_error += pow((original_frame.at<uchar>(i, j) - new_frame.at<uchar>(i, j)), 2);
            
            mean_squared_error /= (height * width);

            return mean_squared_error;
        }
};


#endif