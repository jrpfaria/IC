#ifndef frame_h
#define frame_h

class frame{
    private:
        int width;
        int height;
        char* pixels;

        // \brief To make a grey scale image, we set the U and V values to 127
        int uv_values = 127;
    
    public:
        frame(int width, int height){
            this->width = width;
            this->height = height;

            char* pixels = new char[width * height];
        }

        // \brief Set the pixel at (x, y) to the given char value (pixel)
        void set_pixel(int x, int y, char pixel){
            int index = (y * width + x);
            pixels[index] = pixel;
        }

        int get_width(){
            return width;
        }

        int get_height(){
            return height;
        }

        char* get_pixels(){
            return pixels;
        }

        // \brief Get the pixel at (x, y)
        char get_pixel(int x, int y){
            int index = (y * width + x);
            return pixels[index];
        }

};

#endif;