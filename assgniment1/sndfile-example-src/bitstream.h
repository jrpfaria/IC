#ifndef BITSTREAM
#define BITSTREAM

#include <vector>
#include <math.h>
#include <fstream>
#include <cassert>

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

        vector<unsigned char> read(int n) {
            assert (n>=0&&n<=64);
            vector<unsigned char> result;
            result.resize(n);
            int bytes = floor(n/8);
            int remaining = n%8;
            unsigned char bits;
            unsigned char b;
            int s;
            int i;
            for (i = 0; i < bytes; i++) {
                bits = (*f).get();
                s = 8;
                for (int j = 0; j < 8; j++) {
                    s--;
                    b = (bits>>s)&1;
                    result[(i*8)+j] = b;
                }
            }
            if (remaining>0) {
                bits = (*f).get();
                s = 8;
                for (int j = 0; j < remaining; j++) {
                    s--;
                    b = (bits>>s)&1;
                    result[(i*8)+j] = b;
                }
            }
            return result;
        }

        void write(vector<unsigned char> bits){
            int n = bits.size();
            for (int i = 0; i < n; i++) {
                unsigned char bit = bits[i] & 1;
                int shift = 7 - (i % 8); 
                unsigned char mask = bit << shift;
                if (i % 8 == 0) {
                    if (i > 0) {    
                        f->put(bits[i - 8]);
                    }
                }
                bits[i / 8] |= mask;
            }
        }
    };

#endif