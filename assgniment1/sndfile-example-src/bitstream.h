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

                // Quando maximo array (full_byte) completo dar reset
                if (index_byte == 8){
                    
                    // passar full_byte (array de bits) para Byte
                    
                    //char full_byte = 
                    //file.write(&full_byte,1)
                    index_byte = 0;
                }

                // Começar array correspondente ao byte
                if (index_byte == 0){
                    full_byte = vector<int>(8);
                }
                buffer[index_byte] = bits[bit_counter]

                index_byte++;
                bit_counter++;
            }
        }
    };

#endif