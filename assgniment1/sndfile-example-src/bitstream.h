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

        int index_byte;

    public:
        BitStream(fstream* f) {
            this->f = f;
            index_byte = 0;
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

        void write(vector<int> bits){

            int bit_counter = 0;

            for (int n = bits.size(); n > 0; n--) {

                // Create byte with bits aray(when full) and add it to file 
                if (index_byte == 8){
                    
                    // create a char byte with 
                    char byte = 0;
                    for (int i = 0; i < 8; i++) {
                        byte = (byte << 1) | full_byte[i];
                    }
                    file.write(&byte, 1);
                    index_byte = 0;
                }

                // Reset the byte array when after puting its values in the file
                if (index_byte == 0){
                    full_byte = vector<int>(8);
                }

                // put bit in array (creating byte)
                full_byte[index_byte] = bits[bit_counter]

                index_byte++;
                bit_counter++;
            }

            // If there are remaining bits in the last byte, write it
            if (index_byte > 0) {
                char byte = 0;
                for (int i = 0; i < 8; i++) {
                    byte = (byte << 1) | full_byte[i];
                }
                file.write(&byte, 1);
            }
        }
    };

#endif