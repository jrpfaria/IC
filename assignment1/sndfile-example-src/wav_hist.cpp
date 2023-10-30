#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"
#include <string.h>
#include <math.h>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <channel> [bin size]\n";
		return 1;
	}

	int bin_size;
	if(argc == 4) {
		try {
			bin_size = stoi(argv[3]);
		}
		catch (const exception &) {
			cerr << "Error: invalid bin size\n";
			return 1;
		}
	}
	else bin_size = 0;

	SndfileHandle sndFile { argv[1] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	int channel;
	try {
		channel = { stoi(argv[2]) };
		if(channel >= sndFile.channels()) {
			cerr << "Error: invalid channel requested\n";
			return 1;
		}
	}
	catch (const exception &) {
		if (!strcmp(argv[2], "left")) {
			channel = 0;
		}
		else if (!strcmp(argv[2], "right")) {
			channel = 1;
		}
		else if (!strcmp(argv[2], "mid")) {
			channel = 2;
		}
		else if (!strcmp(argv[2], "side")) {
			channel = 3;
		}
		else {
			cerr << "Error: invalid channel requested\n";
			return 1;
		}
	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile, int(pow(2, bin_size)) };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		hist.update(samples);
	}

	hist.dump(channel);
	return 0;
}

