#include <iostream>
#include <vector>
#include "golomb.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  
  if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <int>"<< endl;
        return 1;
    }
  
  // Create a GolombCoder with the parameter m = 4.
  Golomb coder(4);

  // Encode the integer 15.
  vector<bool> bits = coder.encode(stoi(argv[1]));

  for (int i = 0; i < bits.size(); i++) {
    cout << bits[i] << " ";
  }
  // Decode the sequence of bits.
  int decoded_x = coder.decode(bits);

  // Print the decoded value.
  cout << decoded_x << endl;

  return 0;
}