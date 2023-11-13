#include <iostream>
#include <vector>
#include "ppm_effects.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace ppmeffects;

int main(int argc, char *argv[]) {
    // check number of args
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <input file> <output file> <channels> <effect> <extra>" << endl;
        return 1;
    }

    // read input file
    Mat inputImage = imread(argv[1]);
        if (inputImage.empty()) {
            std::cout << "Error reading input image file " << argv[1] << std::endl;
            return 1;
        }

    string effect = argv[4];

    Mat outputImage(inputImage.size(), CV_8UC3);

    if (effect == "extract")
    {
        if (argv[3][0] != 'b' && argv[3][0] != 'g' && argv[3][0] != 'r') {
            cerr << "Invalid channel! Available: b, g and r" << endl;
            return 1;
        }
        outputImage = extract(inputImage, argv[3][0]);
    }

    else if (effect == "negate")
    {
        outputImage = negative(inputImage);
    }

    else if (effect == "mirror")
    {
        outputImage = mirror(inputImage, argv[5][0]);
    }

    else if (effect == "rotate")
    {
        int angle = stoi(argv[5]);
        if (angle % 90 != 0) {
            cerr << "Degrees to rotate must be a multiple of 90" << endl;
            return 1;
        }
        outputImage = rotate(inputImage,angle);
    }

    else if (effect == "light")
    { 
        outputImage = changeIntensity(inputImage,stod(argv[5]));
    }
    else
    {
        cerr << "Invalid effect! Available: extract, negate, mirror, rotate and light" << endl;
        return 1;
    }
    
    imwrite(argv[2], outputImage);

    // display result
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", outputImage);
    waitKey(0);

    return 0;
}