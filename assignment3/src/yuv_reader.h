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

enum class ColorSpace {
    C420jpeg,
    C420paldv,
    C420mpeg2,
    C420,
    C422,
    C444,
    C444alpha,
    mono,
};

enum class Interlace {
    Ip,
    It,
    Ib,
    Im,
};

class yuv_reader
{
    private:
        std::ifstream file;
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
            frame_rate[0] = stoi(line.substr(line.find('F') + 1));
            frame_rate[0] = stoi(line.substr(line.find(':') + 1));

            // if color space is not defined set it to 420
            if (!strchr(line.c_str(), 'C'))
                color_space = "420";
                // color_space = ColorSpace::C420;
            else
            {
                // extract the color space
                color_space = line.substr(line.find('C') + 1);
                color_space = color_space.substr(0, color_space.find(' '));

                // std::string color_space_str = line.substr(line.find('C') + 1);
                // color_space_str = color_space_str.substr(0, color_space_str.find(' '));

                // if (color_space_str == "420jpeg")
                //     color_space = ColorSpace::C420jpeg;
                // else if (color_space_str == "420paldv")
                //     color_space = ColorSpace::C420paldv;
                // else if (color_space_str == "420mpeg2")
                //     color_space = ColorSpace::C420mpeg2;
                // else if (color_space_str == "420")
                //     color_space = ColorSpace::C420;
                // else if (color_space_str == "422")
                //     color_space = ColorSpace::C422;
                // else if (color_space_str == "444")
                //     color_space = ColorSpace::C444;
                // else if (color_space_str == "444alpha")
                //     color_space = ColorSpace::C444alpha;
                // else if (color_space_str == "mono")
                //     color_space = ColorSpace::mono;

            }

            // if interlace is defined extract it else set it to progressive
            if (!strchr(line.c_str(), 'I'))
                interlace = Interlace::Ip;
            else
                interlace = line.substr(line.find('I') + 1, 1);

                // std::string interlace_str = line.substr(line.find('I') + 1, 1);

                // if (interlace_str == "Ip")
                //     interlace = Interlace::Ip;
                // else if (interlace_str == "It")
                //     interlace = Interlace::It;
                // else if (interlace_str == "Ib")
                //     interlace = Interlace::Ib;
                // else if (interlace_str == "Im")
                //     interlace = Interlace::Im;

            // if the aspect ratio is defined extract it else set it to 1:1
            if (!strchr(line.c_str(), 'A'))
            {
                aspect_ratio[0] = 1;
                aspect_ratio[1] = 1;
            }
            else
            {
                std::string aspect_ratio = line.substr(line.find('A') + 1);

                aspect_ratio[0] = std::stoi(aspect_ratio.substr(0, aspect_ratio.find(':')));
                aspect_ratio[1] = std::stoi(aspect_ratio.substr(aspect_ratio.find(':') + 1));
            }

            
            frame_count = 0;
            while (std::getline(file, line))
                if (line.find("FRAME") != std::string::npos)
                    frame_count++;
            file.clear();
            file.seekg(0);
        }

        Mat get_frame(int n) {
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