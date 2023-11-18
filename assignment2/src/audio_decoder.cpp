#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include "bitstream.h"
#include "golomb.h"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

	BitStream bitstreamInput = BitStream(argv[argc-2], 1);

	vector<unsigned char> bits;
	int size = bitstreamInput.size()*8;
    int chunks = size/64;
    for (int i = 0; i < chunks; i++) {
        std::vector<unsigned char> read = bitstreamInput.read(64);
		for (int i = 0; i < 64; i++) {
			bits.push_back(read[i]);
		}
	}
    int rem = size%64;
    if (rem>0) {
        std::vector<unsigned char> read = bitstreamInput.read(rem);
		for (int i = 0; i < rem; i++) {
			bits.push_back(read[i]);
		}
    }
	
	int cursor = 0;
	size_t m = 0;
	for (int i = 0; i < 16; i++) {
		m += bits[cursor++]<<(15-i);
	}

	size_t nChannels = 0;
	for (int i = 0; i < 16; i++) {
		nChannels += bits[cursor++]<<(15-i);
	}

	size_t nFrames = 0;
	for (int i = 0; i < 32; i++) {
		nFrames += bits[cursor++]<<(31-i);
	}

	int samplerate = 0;
	for (int i = 0; i < 16; i++) {
		samplerate += bits[cursor++]<<(15-i);
	}
	cout << nChannels << endl;
	cout << samplerate << endl;
	SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16,
	  int(nChannels), samplerate };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

	vector<vector<bool>> predG;
	int b = ceil(log2(m));
	int u = (1 << b) - m;
	for (int i = 0; i < int(nChannels * nFrames); i++) {
		vector<bool> value;
		while (true) {
			value.push_back(bits[cursor++]);
			if (!bits[cursor-1]) {
				int r = 0;
				for (int j = 1; j < b; j++) {
					r = (r << 1) | bits[cursor];
					value.push_back(bits[cursor++]);
				}
				if (r>=u) {
					r = (r << 1) | bits[cursor];
					value.push_back(bits[cursor++]);
				}
				predG.push_back(value);
				break;
			}
		}
	}

	Golomb g = Golomb(m, 0);
	vector<short> samples(nChannels * nFrames);
	samples[0] = g.decode(predG[0]);
	for (int i = 1; i < int(nChannels * nFrames); i++) {
		samples[i] = samples[i-1] + g.decode(predG[i]);
	}

	sfhOut.writef(samples.data(), nFrames);
	return 0;
}
