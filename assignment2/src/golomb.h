#ifndef GOLOMB_H
#define GOLOMB_H

#include <iostream>
#include <vector>
#include <math.h>
#include "bitstream.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Golomb {
    private:
        BitStream &bs;
        int m;
        bool method = 1; // sign and magnitude; false = two's complement

    public:
        Golomb(BitStream& _bs, int m, bool method):
        bs(_bs)
        {
            this->m = m;
            this->method = method;
        }

        void encode(int i) {
            int q, r;

            // Encode the sign bit first.
            if (method)
                bs.write(i < 0);
            else
                i = (i < 0) ? abs(i << 1) - 1 : (i << 1);
            
            // Encode the absolute value of i.
            i = abs(i);
            q = floor(i / m);
            r = i % m;

            // Encode the quotient: unary code.
            for (int j = 0; j < q; j++)
                bs.write(1);
            bs.write(0);

            // Encode the remainder: binary code
            // Check the need to use truncated binary.
            int b = ceil(log2(m));
            int u = (1 << b) - m;
            
            // If truncated binary is used
            // Check if the remainder is greater or equal to 2^b - m.
            (r < u) ? bs.write(r, b-1) : bs.write(r+u, b);
        }

        // Decodes a sequence of bits into an integer.
        int decode() {
            int i = 0;

            // Decode the signal of the integer.
            int s = 1;
            if (method){
                s = bs.read() ? -1 : 1;
            }

            // Decode the absolute value of the integer.
            int q = 0, r = 0;
            
            // Decode the quotient from the unary code.
            while (bs.read()) q++;

            // Decode the remainder from the binary code.
            // Check if truncated binary is used.
            int b = ceil(log2(m));
            int u = (1 << b) - m;
            
            for (int j = 0; j < b-1; j++)
                r = (r << 1) | bs.read();
            if (r>=u)
                r = ((r << 1) | bs.read()) - u;

            // Reconstruct the integer.
            i = s * (q*m + r);

            if (!method){
                s = i % 2;
                i = s == 1 ? -((i + 1) >> 1) : i >> 1;
            }

            return i;
        }
};


#endif