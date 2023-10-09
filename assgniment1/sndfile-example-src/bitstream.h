#ifndef BITSTREAM
#define BITSTREAM

#include <iostream>
#include <vector>
#include <math.h>
#include <sndfile.hh>
#include <fstream>

using namespace std;

class BitStream {
    private: 
        fstream* f;

    public:
        BitStream(fstream* f) {
            this->f = f;
        }
        
        void write(int value, int bits) {
            int mask = 1 << (bits - 1);
            for (int i = 0; i < bits; i++) {
                int bit = (value & mask) >> (bits - i - 1);
                *f << bit;
                mask >>= 1;
            }
        }

        void read(int* value, int bits) {
            *value = 0;
            for (int i = 0; i < bits; i++) {
                int bit;
                *f >> bit;
                *value = (*value << 1) | bit;
            }
        }
};

#endif