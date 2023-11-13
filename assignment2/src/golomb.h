#ifndef GOLOMB_H
#define GOLOMB_H

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Golomb {
    private:

        int m;
        bool sign_and_magnitude = true;

    public:
        Golomb(int m){
            this->m = m;
        }

        vector<bool> encode(int x) {
            vector<bool> bits;

            // Encode the sign bit first.
            bits.push_back(x < 0);

            // Encode the absolute value of x.
            int abs_x = abs(x);
            int quotient = abs_x / m;
            int remainder = abs_x % m;

            // Encode the quotient as a unary code.
            for (int i = 0; i < quotient; i++) {
                bits.push_back(true);
            }
            bits.push_back(false);

            // Encode the remainder in binary.
            for (int i = m - 1; i >= 0; i--) {
                bits.push_back((remainder >> i) & 1);
            }

            return bits;
        }

        // Decodes a sequence of bits into an integer.
        int decode(vector<bool> bits) {
            int x = 0;

            // Decode the sign bit first.
            x = bits[0] ? -1 : 1;

            // Decode the absolute value of x.
            int quotient = 0;
            int remainder = 0;

            // Decode the quotient from the unary code.
            int i = 1;
            while (bits[i]) {
                quotient++;
                i++;
            }

            // Decode the remainder from the binary code.
            for (int j = 0; j < m; j++)
                remainder |= bits[i + j] << (m - j - 1);
            

            x *= quotient * m + remainder;

            return x;
        }

        // Sets the Golomb parameter, m.
        void set_m(int m) {
            m = m;
        }

        // Sets the sign and magnitude encoding mode.
        void set_sign_and_magnitude(bool sign_and_magnitude) {
            this->sign_and_magnitude = sign_and_magnitude;
        }
};


#endif