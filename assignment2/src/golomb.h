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
        bool method = 1; // sign and magnitude; false = two's complement

    public:
        Golomb(int m, bool method){
            this->m = m;
            this->method = method;
        }

        vector<bool> encode(int i) {
            vector<bool> bits;
            int q, r, b;

            // Encode the sign bit first.
            if (method)
                bits.push_back(i < 0);
            else 
                i = (i < 0) ? abs(i << 1) - 1 : (i << 1);
            
            // Encode the absolute value of i.
            i = abs(i);
            q = floor(i / m);
            r = i % m;

            // Encode the quotient: unary code.
            for (int j = 0; j < q; j++)
                bits.push_back(1);
            bits.push_back(0);

            // Encode the remainder: binary code
            // Check the need to use truncated binary.
            bool t = log2(m) != ceil(log2(m));
            b = ceil(log2(m));
            
            // If truncated binary is used
            // Check if the remainder is greater or equal to 2^b - m.
            if (t && r >= (1 << b) - m)
                r = r + (1 << b++) - m;

            // Encode the remainder with the appropriate number of bits.
            int j = t ? b - 2 : b - 1;
            for (;j >= 0; j--)
                bits.push_back((r >> j) & 1);

            return bits;
        }

        // Decodes a sequence of bits into an integer.
        int decode(vector<bool> bits) {
            int i = 0;

            // Decode the sign bit first.
            i = bits[0] ? -1 : 1;

            // Decode the absolute value of x.
            int q = 0;
            int r = 0;

            // Decode the quotient from the unary code.
            int j = 1;
            while (bits[j]) {
                q++;
                j++;
            }

            // Decode the remainder from the binary code.
            for (int j = 0; j < m; j++)
                r |= bits[i + j] << (m - j - 1);
            

            i *= q * m + r;

            return i;
        }
};


#endif