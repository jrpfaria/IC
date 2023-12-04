#include <iostream>
#include <vector>
#include "ppm_effects.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace ppmeffects;

int main(int argc, char *argv[]) {
    // check number of args
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input file> <output file> <effect> <extra>" << endl;
        return 1;
    }

    // read input file
    Mat inputImage = imread(argv[1]);
        if (inputImage.empty()) {
            std::cout << "Error reading input image file " << argv[1] << std::endl;
            return 1;
        }

    string effect = argv[3];
    string extra = (argc > 4) ? argv[4] : "";
    string outputFileName = argv[2];

    Mat outputImage(inputImage.size(), CV_8UC3);

    if (effect == "extract")
    {
        char channel = extra[0];
        if (channel != 'b' && channel != 'g' && channel != 'r') {
            cerr << "Invalid channel! Available: b, g and r" << endl;
            return 1;
        }
        outputImage = extract(inputImage, channel);
    }

    else if (effect == "negative")
    {
        outputImage = negative(inputImage);
    }

    else if (effect == "mirror")
    {
        char axis = extra[0];
        if (axis != 'v' && axis != 'h'){
            cerr << "Invalid axis! Available: v(vertically) or h(horizontally)" << endl;
            return 1;
        }
        outputImage = mirror(inputImage, axis);
    }

    else if (effect == "rotate")
    {
        int angle = stoi(extra);
        if (angle % 90 != 0) {
            cerr << "Degrees to rotate must be a multiple of 90" << endl;
            return 1;
        }
        outputImage = rotate(inputImage,angle);
    }

    else if (effect == "light")
    { 
        float intensity = stod(extra);
        if (intensity > '2' || intensity < 0) {
            cerr << "Invalid operation! Light value should be in the [0; 2] interval" << endl;
            return 1;
        }
        outputImage = changeIntensity(inputImage,intensity);
    }

    else if (effect == "gray")
    {
        outputImage = grayScale(inputImage);
    }

    else
    {
        cerr << "Invalid effect! Available: extract, negative, mirror, rotate and light" << endl;
        return 1;
    }
    
    if (effect == "extract")
        cvtColor(outputImage, outputImage, COLOR_GRAY2BGR);
    imwrite(outputFileName, outputImage);

    return 0;
}