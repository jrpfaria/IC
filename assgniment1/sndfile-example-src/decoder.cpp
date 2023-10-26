#include <iostream>
#include "bitstream.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
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
        fclose(fopen(argv[2], "a"));
    }
    
    BitStream bitstreamInput { &fileInput };
    BitStream bitstreamOutput { &fileOutput };
    int bytes = bitstreamInput.size();
    vector<unsigned char> write = {0, 0, 1, 1, 0, 0, 0, 0};
    for (int i = 0; i < bytes; i++) {
        vector<unsigned char> read = bitstreamInput.read(8);
        for (int j = 0; j < 8; j++) {
            write[7] = read[j];
            bitstreamOutput.write(write);
        }
    }
    bitstreamInput.close();
    bitstreamOutput.close();
    return 0;
}
