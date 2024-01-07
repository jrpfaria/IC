#ifndef YUV_READER_H
#define YUV_READER_H

#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <vector>
#include <cstring>
#include <opencv2/opencv.hpp>

using namespace cv;

enum ColorSpace {
    C420jpeg,
    C420paldv,
    C420mpeg2,
    C420,
    C422,
    C444,
    C444alpha,
    Cmono,
};

enum Interlace {
    Ip,
    It,
    Ib,
    Im,
};

class yuv_writer
{
    private:
        std::fstream file;
        int resolution[2];
        Interlace interlace;
        int aspect_ratio[2];
        int frame_rate[2];
        int frame_count;
        ColorSpace color_space;

    public:
        yuv_writer(const std::string &file_name)
        {
            file.open(file_name, std::ios::out | std::ios::binary | std::ios::app);
        }

        void write_header()
        {
            // Write YUV file header
            file << "YUV4MPEG2" << " W" << this->resolution[0] << " H" << this->resolution[1] << " F" << frame_rate[0] << ":" << frame_rate[1] << " ";

            if (color_space == ColorSpace::C420jpeg)
                file << "C420jpeg";
            else if (color_space == ColorSpace::C420paldv)
                file << "C420paldv";
            else if (color_space == ColorSpace::C420mpeg2)
                file << "C420mpeg2";
            else if (color_space == ColorSpace::C420)
                file << "C420";
            else if (color_space == ColorSpace::C422)
                file << "C422";
            else if (color_space == ColorSpace::C444)
                file << "C444";
            else if (color_space == ColorSpace::C444alpha)
                file << "C444alpha";
            else if (color_space == ColorSpace::Cmono)
                file << "Cmono";
            
            file << " ";

            if (interlace == Interlace::Ip)
                file << "Ip";
            else if (interlace == Interlace::It)
                file << "It";
            else if (interlace == Interlace::Ib)
                file << "Ib";
            else if (interlace == Interlace::Im)
                file << "Im";
            
            file << " A" << aspect_ratio[0] << ":" << aspect_ratio[1];
            
            file << std::endl;
        }

        void write_frame(const Mat &frame)
        {
            // Write the "FRAME" header
            file << "FRAME\n";

            // Write YUV frame data
            for (int h = 0; h < frame.rows; h++)
            {
                for (int w = 0; w < frame.cols; w++)
                {
                    char pixel_value = frame.at<uchar>(h, w);
                    file.write(&pixel_value, 1);
                }
            }
            frame_count++;
        }

        void close() {
            file.close();
        }

        void set_resolution(int* resolution)
        {
            this->resolution[0] = resolution[0];
            this->resolution[1] = resolution[1];
        }

        void set_interlace(Interlace interlace)
        {
            this->interlace = interlace;
        }

        void set_aspect_ratio(int* aspect_ratio)
        {
            this->aspect_ratio[0] = aspect_ratio[0];
            this->aspect_ratio[1] = aspect_ratio[1];
        }

        void set_frame_rate(int* frame_rate)
        {
            this->frame_rate[0] = frame_rate[0];
            this->frame_rate[1] = frame_rate[1];
        }

        void set_frame_count(int frame_count)
        {
            this->frame_count = frame_count;
        }

        void set_color_space(ColorSpace color_space)
        {
            this->color_space = color_space;
        }

        // \brief Takes a Mat object and converts it to a vector of unsigned chars in YUV format
        // TODO: usar bitstream
        std::vector<unsigned char> mat_to_yuv(Mat frame, ColorSpace color_space)
        {
            std::vector<unsigned char> frame_data;
            
            // Write Y data
            for (int h = 0; h < frame.rows; h++)
            {
                for (int w = 0; w < frame.cols; w++)
                {
                    frame_data.push_back(frame.at<uchar>(h, w));
                }
            }

            // Write U and V data
            switch (color_space)
            {
                case C420mpeg2:
                case C420paldv:
                case C420jpeg:
                case C420: // 4:2:0 (width*height*3/2)
                    for (int i = 0; i < (frame.rows*frame.cols) / 2; i++)
                        frame_data.push_back((unsigned char)127);
                    break;

                case C422: // 4:2:2 (width*height*2)
                    for (int i = 0; i < frame.rows*frame.cols; i++)
                        frame_data.push_back((unsigned char)127);
                    break;

                case C444: // 4:4:4 (width*height*3)
                    for (int i = 0; i < frame.rows*frame.cols; i++)
                        frame_data.push_back((unsigned char)127),
                        frame_data.push_back((unsigned char)127);
                    break;
                
                case C444alpha: // 4:4:4:4 (width*height*4)
                    for (int i = 0; i < frame.rows*frame.cols; i++)
                        frame_data.push_back((unsigned char)127),
                        frame_data.push_back((unsigned char)127);
                    for (int i = 0; i < frame.rows*frame.cols; i++)
                        frame_data.push_back((unsigned char)255);
                    break;

                default:
                    break;
            }

            return frame_data;
        }
        
};

#endif