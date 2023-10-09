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
    int r;
    bitstream.read(&r, 1);
    cout << r << "\n";
    return 0;
}
