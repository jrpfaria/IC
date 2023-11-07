#include <iostream>
#include <vector>
#include "ppm_effects.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

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

    PPMEffects result = PPMEffects(argv[3][0]);

    Mat outputImage(inputImage.size(), CV_8UC3);

    if (effect == "extract")
    {
        outputImage = result.extract(inputImage);
    }

    else if (effect == "negate")
    {
        outputImage = result.negative(inputImage);
    }

    else if (effect == "mirror")
    {
        outputImage = result.mirror(inputImage, argv[5][0]);
    }

    else if (effect == "rotate")
    {
        int angle = stoi(argv[5]);
        if (angle % 90 != 0) {
            cerr << "Degrees to rotate must be a multiple of 90" << endl;
            return 1;
        }
        outputImage = result.rotate(inputImage,angle);
    }

    else if (effect == "light")
    { 
        outputImage = result.changeIntensity(inputImage,stod(argv[5]));
    }
    else
    {
        cerr << "Invalid effect! Available: extract, negate, mirror, rotate and light" << endl;
        return 1;
    }
    
    // TO DO: switch case with argument in order to select which effect to use
    // extract channels
    // PPMEffects result = PPMEffects(argv[3][0]);
    // image = result.extract(image);

    // negative
    // image = PPMEffects::negative(image);

    // mirror image
    // image = PPMEffects::mirror(image, argv[4][0]);

    // rotate image
    // image = PPMEffects::rotate(image, stoi(argv[4]));
    // TO DO: fix rotate for |rotation| > 90

    // change light intensity
    // image = PPMEffects::changeIntensity(image, stof(argv[4]));

    // output image to ppm file
    imwrite(argv[2], outputImage);

    // display result
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", outputImage);
    waitKey(0);

    return 0;
}