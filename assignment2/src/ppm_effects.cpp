#include <iostream>
#include <vector>
#include "ppm_effects.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    // check number of args
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input file> <output file> <effect> <parameters>" << endl;
        return 1;
    }

    return 0;
}
