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

class yuv_reader
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
        yuv_reader(const std::string &file_name)
        {
            std::string line;
            file.open(file_name);

            if (!file.is_open())
                throw std::runtime_error(file_name + " - could not be opened.");
            
            std::getline(file, line);
            
            // get resolution and frame rate
            resolution[0] = stoi(line.substr(line.find('W') + 1));
            resolution[1] = stoi(line.substr(line.find('H') + 1));
            
            String frame_rate_str = line.substr(line.find('F'));
            frame_rate[0] = stoi(frame_rate_str.substr(1, frame_rate_str.find(':') - 1));
            frame_rate[1] = stoi(frame_rate_str.substr(frame_rate_str.find(':') + 1, frame_rate_str.find(' ') - 1));

            // if color space is not defined set it to 420
            if (!strchr(line.c_str(), 'C'))
                color_space = ColorSpace::C420;
            else
            {
                // extract the color space
                String color_space_str = line.substr(line.find('C'));
                color_space_str = color_space_str.substr(0, color_space_str.find(' '));

                if (color_space_str == "C420jpeg")
                    color_space = ColorSpace::C420jpeg;
                else if (color_space_str == "C420paldv")
                    color_space = ColorSpace::C420paldv;
                else if (color_space_str == "C420mpeg2")
                    color_space = ColorSpace::C420mpeg2;
                else if (color_space_str == "C420")
                    color_space = ColorSpace::C420;
                else if (color_space_str == "C422")
                    color_space = ColorSpace::C422;
                else if (color_space_str == "C444")
                    color_space = ColorSpace::C444;
                else if (color_space_str == "C444alpha")
                    color_space = ColorSpace::C444alpha;
                else if (color_space_str == "Cmono")
                    color_space = ColorSpace::Cmono;
            }

            // if interlace is defined extract it else set it to progressive
            if (!strchr(line.c_str(), 'I'))
                interlace = Interlace::Ip;
            else {
                String interlace_str = line.substr(line.find('I'));
                interlace_str = interlace_str.substr(0, interlace_str.find(' '));

                if (interlace_str == "Ip")
                    interlace = Interlace::Ip;
                else if (interlace_str == "It")
                    interlace = Interlace::It;
                else if (interlace_str == "Ib")
                    interlace = Interlace::Ib;
                else if (interlace_str == "Im")
                    interlace = Interlace::Im;
            }

            // if the aspect ratio is defined extract it else set it to 1:1
            if (!strchr(line.c_str(), 'A'))
            {
                aspect_ratio[0] = 1;
                aspect_ratio[1] = 1;
            }
            else
            {
                String aspect_ratio_str = line.substr(line.find('A'));
                aspect_ratio[0] = stoi(aspect_ratio_str.substr(1, aspect_ratio_str.find(':') - 1));
                aspect_ratio[1] = stoi(aspect_ratio_str.substr(aspect_ratio_str.find(':') + 1, aspect_ratio_str.find(' ') - 1));
            }

            
            frame_count = 0;
            while (std::getline(file, line))
                if (line.find("FRAME") != std::string::npos)
                    frame_count++;
            
            file.clear();
            file.seekg(0);
        }

        Mat get_frame(int n) {
            file.clear();
            file.seekg(0);
            Mat pixels = Mat::zeros(resolution[1], resolution[0], CV_8UC1);
            int count = 0;
            std::string line;
            while (std::getline(file, line)) {
                if (line.find("FRAME") != std::string::npos) {
                    if (count++==n) {
                        for (int h = 0; h < resolution[1]; h++) {
                            for (int w = 0; w < resolution[0]; w++) {
                                char c = 0;
                                file.read(&c, 1);
                                pixels.at<uchar>(h,w) = c;
                            }
                        }
                        break;
                    }
                }
            }
            return pixels;
        }

        int *get_resolution()
        {
            return resolution;
        }

        Interlace get_interlace()
        {
            return interlace;
        }

        int *get_aspect_ratio()
        {
            return aspect_ratio;
        }

        int *get_frame_rate()
        {
            return frame_rate;
        }

        int get_frame_count()
        {
            return frame_count;
        }

        ColorSpace get_color_space()
        {
            return color_space;
        }
        
};

#endif