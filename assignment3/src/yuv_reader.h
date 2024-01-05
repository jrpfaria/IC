#include <string>
#include <fstream>
#include <regex>
#include <iostream>

class yuv_reader
{
    private:
        std::ifstream file;
        int resolution[2];
        std::string interlace;
        int aspect_ratio[2];
        int frame_rate[2];
        int frame_count;
        std::string color_space;

    public:
        yuv_reader(const std::string &file_name)
        {
            std::string line;
            file.open(file_name);
            if (!file.is_open())
                throw std::runtime_error(file_name + " - could not be opened.");
            
            

            std::getline(file, line);
            sscanf(line.c_str(), "YUV4MPEG2 W%d H%d F%d:%d",
                &resolution[0], &resolution[1],
                &frame_rate[0], &frame_rate[1]);

            // check if the color space is defined
            if (!strchr(line.c_str(), 'C'))
                color_space = "420";
            else
            {
                // extract the color space that starts with 'C'
                color_space = line.substr(line.find('C') + 1);
                std::cout << "here" << std::endl;
            }

            // check if the interlace is defined
            if (!strchr(line.c_str(), 'I'))
                interlace = "p";
            else
            {
                // extract the interlace that starts with 'I' with the next character
                interlace = line.substr(line.find('I') + 1, 1);
            }

            // check if the aspect ratio is defined
            if (!strchr(line.c_str(), 'A'))
            {
                aspect_ratio[0] = 1;
                aspect_ratio[1] = 1;
            }
            else
            {
                // extract the aspect ratio that starts with 'A'
                std::string aspect_ratio = line.substr(line.find('A') + 1);

                aspect_ratio[0] = std::stoi(aspect_ratio.substr(0, aspect_ratio.find(':')));
                aspect_ratio[1] = std::stoi(aspect_ratio.substr(aspect_ratio.find(':') + 1));
            }

            // count the number of frames
            frame_count = 0;
            while (std::getline(file, line))
                if (line.find("FRAME") != std::string::npos)
                    frame_count++;
        }

        int *get_resolution()
        {
            return resolution;
        }

        std::string get_interlace()
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

        std::string get_color_space()
        {
            return color_space;
        }
};
