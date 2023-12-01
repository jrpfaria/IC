#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "bitstream.h"
#include "golomb.h"

using namespace std;

int main(int argc, char *argv[]) {
	bool verbose { false };
	bool lossy { false };
	double averageBitRate = 0;

	if(argc < 3) {
		cerr << "Usage: audio_encoder [ -v (verbose) ] [ -b averageBitRate ] \n";
		cerr << "               wavFileIn FileOut\n";
		return 1;
	}

	for(int n = 1 ; n < argc ; n++) {
		if(string(argv[n]) == "-v") {
			verbose = true;
			break;
		}
		if(string(argv[n]) == "-b") {
			lossy = true;
			averageBitRate = stod(argv[n+1]);
			break;
		}
	}

	SndfileHandle sfhIn { argv[argc-2] };
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	if(verbose) {
		cout << "Input file has:\n";
		cout << '\t' << sfhIn.frames() << " frames\n";
		cout << '\t' << sfhIn.samplerate() << " samples per second\n";
		cout << '\t' << sfhIn.channels() << " channels\n";
	}

	size_t nChannels { static_cast<size_t>(sfhIn.channels()) };
	size_t nFrames { static_cast<size_t>(sfhIn.frames()) };

	vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);

	vector<int> pred;
	pred.push_back(samples[0]);
	for (int i = 1; i < int(nChannels * nFrames); i++) {
		pred.push_back(samples[i]-samples[i-1]);
	}

	int m = Golomb::idealM(pred);

	BitStream bitstreamOutput { argv[argc-1], 0 };
	bitstreamOutput.write(m, 16);
	bitstreamOutput.write(nChannels,16);
	bitstreamOutput.write(nFrames,32);
	bitstreamOutput.write(sfhIn.samplerate(),16);

	Golomb g = Golomb(bitstreamOutput, m, 0);
	for (auto p: pred) {
		g.encode(p);
	}

    bitstreamOutput.close();
	return 0;
}
