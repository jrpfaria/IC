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
	int m = bitstreamInput.readInt(16);
	size_t nChannels = bitstreamInput.readInt(16);
	size_t nFrames = bitstreamInput.readInt(32);
	int samplerate = bitstreamInput.readInt(16);
	
	SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, int(nChannels), samplerate };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

	vector<int> pred;
	Golomb g = Golomb(bitstreamInput, m, 0);
	for (int i = 0; i < int(nChannels * nFrames); i++) {
		pred.push_back(g.decode());
	}

	vector<short> samples(nChannels * nFrames);
	samples[0] = pred[0];
	for (int i = 1; i < int(nChannels * nFrames); i++) {
		samples[i] = samples[i-1] + pred[i];
	}

	sfhOut.writef(samples.data(), nFrames);
	return 0;
}
