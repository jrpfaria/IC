#include <iostream>
#include "bitstream.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <input file>\n";
		return 1;
	}

    fstream file;
    try {
        file = fstream(argv[1], std::fstream::in | std::fstream::out | std::fstream::binary);
    }
    catch (const exception &) {
        cerr << "Error: invalid file";
        return 1;
    }

    BitStream bitstream { &file };
    std::vector<unsigned char> result = bitstream.read(64);
    for (auto b: result) {
        cout << int(b);
    }
    cout << "\n";
    return 0;
}
