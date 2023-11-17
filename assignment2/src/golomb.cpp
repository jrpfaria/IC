#include <iostream>
#include <vector>
#include "golomb.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <int> <m> <method> "<< endl;
    return 1;
  }

  int m = stoi(argv[2]);
  int method = stoi(argv[3]);

  if (m < 1) {
    cerr << "M must be greater than 0" << endl;
    return 1;
  }

  if (method != 0 && method != 1) {
    cerr << "Method must be boolean value 0 (value intervaling) or 1 (signal and magnitude)" << endl;
    return 1;
  }

  // Create a GolombCoder with the parameter m = 4.
  Golomb coder(m, method);

  // Encode the integer 15.
  vector<bool> bits = coder.encode(stoi(argv[1]));

  // for (long unsigned int i = 0; i < bits.size(); i++) {
  //   cout << bits[i] << " ";
  // }

  // Decode the sequence of bits.
  int decoded_x = coder.decode(bits);

  // Print the decoded value.
  cout << decoded_x << endl;

  return 0;
}