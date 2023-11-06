#include <iostream>
#include <vector>
#include "ppm_effects.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    // check number of args
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file> <channels>" << endl;
        return 1;
    }

    // read input file
    Mat image = imread(argv[1], IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Could not open or find the image" << endl;
        return 1;
    }

    // extract channels
    // PPMEffects result = PPMEffects(argv[3][0]);
    // image = result.extract(image);

    image = PPMEffects::mirror(image, argv[4][0]);

    // output image to ppm file
    imwrite(argv[2], image);

    // display result
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", image);
    waitKey(0);

    return 0;
}