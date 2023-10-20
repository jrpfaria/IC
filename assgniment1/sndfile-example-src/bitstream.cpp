#include <iostream>
#include "bitstream.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

    fstream fileInput;
    try {
        fileInput = fstream(argv[1], std::fstream::in | std::fstream::binary);
    }
    catch (const exception &) {
        cerr << "Error: invalid input file";
        return 1;
    }

    fstream fileOutput;
    try {
        fileOutput = fstream(argv[2], std::fstream::out | std::fstream::binary);
    }
    catch (const exception &) {
        fopen(argv[2], "a");
    }
    
    BitStream bitstreamInput { &fileInput };
    BitStream bitstreamOutput { &fileOutput };
    int bits = bitstreamInput.size()*8;
    if (bits >= 64) {
        for (int i = 0; i < bits; i++) {
            std::vector<unsigned char> result = bitstreamInput.read(64);
            bitstreamOutput.write(result);
        }
    }
    int rem = bits%64;
    if (rem>0) {
        std::vector<unsigned char> result = bitstreamInput.read(rem);
        bitstreamOutput.write(result);
    }
    return 0;
}
