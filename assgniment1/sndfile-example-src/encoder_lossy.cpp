#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitstream.h"

using namespace std;

int getValueBits(vector<vector<double>> v) {
	int min_value = INT32_MAX;
    int max_value = INT32_MIN;

    for (auto row: v) {
        for (int value: row) {
			if (value < min_value) {
                min_value = int(value);
            }
            if (value > max_value) {
                max_value = int(value);
            }
        }
    }
	
    int valueBits = 0;
    if (abs(min_value)>abs(max_value)) {
        valueBits = static_cast<int>(log2(abs(min_value))) + 1;
    }
	else valueBits = static_cast<int>(log2(max_value)) + 2;
    return valueBits;
}

int main(int argc, char *argv[]) {

	bool verbose { false };
	size_t bs { 1024 };
	double dctFrac { 0.2 };

	if(argc < 3) {
		cerr << "Usage: encoder_lossy [ -v (verbose) ]\n";
		cerr << "               [ -bs blockSize (def 1024) ]\n";
		cerr << "               [ -frac dctFraction (def 0.2) ]\n";
		cerr << "               wavFileIn FileOut\n";
		return 1;
	}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-v") {
			verbose = true;
			break;
		}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-bs") {
			bs = atoi(argv[n+1]);
			break;
		}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-frac") {
			dctFrac = atof(argv[n+1]);
			break;
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

	// Read all samples: c1 c2 ... cn c1 c2 ... cn ...
	// Note: A frame is a group c1 c2 ... cn
	vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);

	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs)) };

	// Do zero padding, if necessary
	samples.resize(nBlocks * bs * nChannels);

	// Vector for holding all DCT coefficients, channel by channel
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));

	// Vector for holding DCT computations
	vector<double> x(bs);

	// Direct DCT
	fftw_plan plan_d = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++)
				x[k] = samples[(n * bs + k) * nChannels + c];

			fftw_execute(plan_d);
			// Keep only "dctFrac" of the "low frequency" coefficients
			for(size_t k = 0 ; k < bs * dctFrac ; k++)
				x_dct[c][n * bs + k] = x[k] / (bs << 1);

		}
    
    fstream fileOutput;
    try {
        fileOutput = fstream(argv[argc-1], std::fstream::out | std::fstream::binary);
    }
    catch (const exception &) {
        fopen(argv[argc-1], "a");
    }
    BitStream bitstreamOutput { &fileOutput };

    vector<unsigned char> bits;

    for (int i = 0; i < 16; i++) {
        bits.push_back((bs>>(15-i))&1);
    }

    for (int i = 0; i < 16; i++) {
        bits.push_back((nChannels>>(15-i))&1);
    }

    for (int i = 0; i < 32; i++) {
        bits.push_back((nFrames>>(31-i))&1);
    }

    for (int i = 0; i < 16; i++) {
        bits.push_back((nBlocks>>(15-i))&1);
    }

    int samplerate = sfhIn.samplerate();
    for (int i = 0; i < 16; i++) {
        bits.push_back((samplerate>>(15-i))&1);
    }

	int valueBits = getValueBits(x_dct);
    for (int i = 0; i < 16; i++) {
        bits.push_back((valueBits>>(15-i))&1);
    }

    for (int i = 0; i < int(nChannels); i++) {
        for (int j = 0; j < int(nBlocks * bs); j++) {
            for (int k = 32-valueBits; k < 32; k++) {
                bits.push_back(((int(x_dct[i][j]))>>(31-k))&1);
            }
        }
    }

    bitstreamOutput.write(bits);
    bitstreamOutput.close();
	return 0;
}

