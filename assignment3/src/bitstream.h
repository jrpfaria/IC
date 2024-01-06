#ifndef BITSTREAM
#define BITSTREAM

#include <vector>
#include <math.h>
#include <fstream>
#include <cassert>

using namespace std;

class BitStream {
    private: 
        fstream f;
        bool mode;
        unsigned char buffer;
        int free = 8;
    
    public:
        BitStream(char* file, bool mode) {
            this->mode = mode;
            if (mode) {
                this->f = fstream(file, std::fstream::in | std::fstream::binary);
            }
            else {
                fclose(fopen(file, "a"));
                this->f = fstream(file, std::fstream::out | std::fstream::binary);
            }
        }
        
        unsigned char read() {
            if (free==8) {
                char c = 0;
                f.read(&c, 1);
                buffer = c;
                free = 0;
            }
            unsigned char bit = (buffer>>(7-free))&1;
            free++;
            return bit;
        }

        vector<unsigned char> read(int n) {
            vector<unsigned char> bits(n);
            for (int i = 0; i < n; i++) {
                bits[i] = read();
            }
            return bits;
        }

        int readInt(int n) {
            int result = 0;
            for (int i = 0; i < n; i++) {
                result = (result<<1)|read();
            }
            return result;
        }

        string readString(int n) {
            string result = "";
            for (int i = 0; i < n; i++) {
                result += f.get();
            }
            return result;
        }

        void write(unsigned char bit) {
            buffer = buffer<<1;
            buffer = buffer|bit;
            free--;
            if (free==0) {
                char c = buffer;
                f.write(&c, 1);
                free = 8;
            }
        }

        void write(vector<unsigned char> bits) {
            int n = bits.size();
            for (int i = 0; i < n; i++) {
                write(bits[i]);
            }
        }

        void write(int value, int n) {
            for (int i = 0; i < n; i++) {
                write((value>>(n-i-1))&1);
            }
        }

        void write(string s) {
            int l = s.length();
            for (int i = 0; i < l; i++) {
                write(s[i],8);
            }
        }

        void close() {
            if (!mode) while (free!=8) write(0);
            f.close();
        }
    };

#endif
