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

        static int idealM(vector<int> values) {
            double e = 0;
            for (auto i: values) {
                int j = (i < 0) ? abs(i << 1) - 1 : (i << 1);
                e += abs(j);
            }
            e /= values.size();
            double p = 1;
            p /= e;
            int m = ceil(-log(2-p)/log(1-p));
            return m;
        }

        int getEncodedLength(int i) {
            int n = 0;
            int q, r;

            if (method)
                n++;
            else
                i = (i < 0) ? abs(i << 1) - 1 : (i << 1);
            
            i = abs(i);
            q = floor(i / m);
            r = i % m;

            n += q+1;

            int b = ceil(log2(m));
            int u = (1 << b) - m;
            (r < u) ? n += b-1 : n += b;

            return n;
        }
        
        void encode(int i) {
            int q, r;

            if (method)
                // Encode the sign bit
                bs.write(i < 0);
            else
                // Positive values at even (2x), negative values at odd (-2x-1)
                i = (i < 0) ? abs(i << 1) - 1 : (i << 1);
            
            // Encode the absolute value of i
            i = abs(i);
            q = floor(i / m);
            r = i % m;

            // Encode the quotient: unary code
            for (int j = 0; j < q; j++)
                bs.write(1);
            bs.write(0);

            // Encode the remainder with the appropriate number of bits
            int b = ceil(log2(m));
            int u = (1 << b) - m;
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

            int q = 0, r = 0;
            
            // Decode the quotient from the unary code.
            while (bs.read()) q++;

            // Decode the remainder with the appropriate number of bits
            int b = ceil(log2(m));
            int u = (1 << b) - m;
            
            for (int j = 0; j < b-1; j++)
                r = (r << 1) | bs.read();
            if (r>=u)
                r = ((r << 1) | bs.read()) - u;

            // Reconstruct the integer.
            i = s * (q*m + r);

            // Even values to positive (x/2), odd values to negative (-(x+1)/2)
            if (!method){
                s = i % 2;
                i = s == 1 ? -((i + 1) >> 1) : i >> 1;
            }

            return i;
        }
};


#endif