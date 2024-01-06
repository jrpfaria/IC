#ifndef frame_h
#define frame_h

#include <opencv2/opencv.hpp>

// Set desired block dimension
#define blockWidth  192 // random for now
#define blockHeight 108 // random for now

using namespace std;
using namespace cv;

class frame{
    private:
        int width;
        int height;
        unsigned char* pixels;

        // \brief To make a grey scale image, we set the U and V values to 127
        int uv_values = 127;
    
    public:
        frame(int width, int height){
            this->width = width;
            this->height = height;

            this->pixels = new unsigned char[width * height];
        }

        // \brief Set the pixel at (x, y) to the given char value (pixel)
        void set_pixel(int x, int y, unsigned char pixel){
            int index = (y * width + x);
            pixels[index] = pixel;
        }

        int get_width(){
            return width;
        }

        int get_height(){
            return height;
        }

        unsigned char* get_pixels(){
            return pixels;
        }

        // \brief Get the pixel at (x, y)
        unsigned char get_pixel(int x, int y){
            int index = (y * width + x);
            return pixels[index];
        }

};

#endif