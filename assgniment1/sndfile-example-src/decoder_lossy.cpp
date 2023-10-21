#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitstream.h"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

	fstream fileInput;
    try {
        fileInput = fstream(argv[argc-1], std::fstream::in | std::fstream::binary);
    }
    catch (const exception &) {
        cerr << "Error: invalid input file\n";
		return 1;
    }
    BitStream bitstreamInput { &fileInput };

	int size = bitstreamInput.size()*8;
	vector<unsigned char> bits = bitstreamInput.read(size);

	size_t bs = 0;
	for (int i = 0; i < 16; i++) {
		bs += bits[i]<<(15-i);
	}

	size_t nChannels = 0;
	for (int i = 0; i < 16; i++) {
		nChannels += bits[16+i]<<(15-i);
	}

	size_t nFrames = 0;
	for (int i = 0; i < 32; i++) {
		nFrames += bits[32+i]<<(32-i);
	}

	size_t nBlocks = 0;
	for (int i = 0; i < 16; i++) {
		nBlocks += bits[70+i]<<(15-i);
	}

	int samplerate = 0;
	for (int i = 0; i < 16; i++) {
		samplerate += bits[86+i]<<(15-i);
	}

	SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16,
	  int(nChannels), samplerate };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

	vector<short> samples(nChannels * nFrames);

	// Do zero padding, if necessary
	samples.resize(nBlocks * bs * nChannels);

	// Vector for holding all DCT coefficients, channel by channel
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));
	int n = 80;
	for (int i = 0; i < int(nChannels); i++) {
        for (int j = 0; j < int(nBlocks * bs); j++) {
            for (int k = 0; k < 32; k++) {
				x_dct[i][j] += bits[n++]<<(31-k);
            }
        }
    }

	// Vector for holding DCT computations
	vector<double> x(bs);

	// Inverse DCT
	fftw_plan plan_i = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++)
				x[k] = x_dct[c][n * bs + k];

			fftw_execute(plan_i);
			for(size_t k = 0 ; k < bs ; k++)
				samples[(n * bs + k) * nChannels + c] = static_cast<short>(round(x[k]));

		}

	sfhOut.writef(samples.data(), nFrames);
	return 0;
}

