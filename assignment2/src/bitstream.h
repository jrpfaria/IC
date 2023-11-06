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
        unsigned char buffer = 0;
        int free = 8;
        int index = 0;
    
    public:
        BitStream(char* fileName) {
            try {
                this->f = fstream(fileName);
            }
            catch (const exception &) {
                fclose(fopen(fileName, "a"));
            }
        }

        ~BitStream() {
            while (free!=8) write(0);
            f.close();
        }

        int size() {
            int current = f.tellg();
            f.clear();
            f.seekg(0);
            int size = f.tellg();
            f.seekg(0, std::ios::end);
            size = int(f.tellg())-size;
            f.clear();
            f.seekg(current);
            return size;
        }

        vector<unsigned char> read(int n) {
            assert (n>=0&&n<=64);
            vector<unsigned char> result;
            result.resize(n);
            int bytes = floor(n/8);
            int remaining = n%8;
            unsigned char bits;
            unsigned char b;
            int i;
            for (i = 0; i < bytes; i++) {
                bits = f.get();
                for (int j = 0; j < 8; j++) {
                    b = (bits>>(7-j))&1;
                    result[(i*8)+j] = b;
                }
            }
            if (remaining>0) {
                bits = f.get();
                for (int j = 0; j < remaining; j++) {
                    b = (bits>>(7-j))&1;
                    result[(i*8)+j] = b;
                }
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
            for (int i = n; i < 32; i++) {
                write((value>>(31-i))&1);
            }
        }

        void write(string s) {
            int l = s.length();
            unsigned char bit;
            for (int i = 0; i < l; i++) {
                for (int j = 0; j < 8; j++) {
                    bit = (s[i]>>(7-j))&1;
                    write(bit);
                }
            }
        }
    };

#endif