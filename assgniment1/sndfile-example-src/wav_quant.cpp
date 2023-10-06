#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]){
    
    // check number of args
    if(argc < 4){
        cerr << "Usage: " << argv[0] << " <input file> <output file> <quantization level>" << endl;
        return 1;
    }

    // get input
    SndfileHandle inputFile { argv[argc-3] };
    // check if correct input file
    if (inputFile.error()){
        cerr << "Invalid input file!\n";
        return 1;
    }

    SndfileHandle outputFile { argv[argc-2], SFM_WRITE, inputFile.format(), inputFile.channels(), inputFile.samplerate() };
    if (not outputFile) return 1;

    int btc = std::atoi(argv[argc-1]);
    if (btc < 1 || btc > 16){
        cerr << "Invalid bits to keep!";
        return 1;
    }

    vector<short> audioSamples(inputFile.frames());
    inputFile.readf(audioSamples.data(), audioSamples.size());

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * inputFile.channels());
    WAVQuant quant { btc };
    while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * inputFile.channels());
		quant.update(samples);
	}
    quant.toFile(outputFile);
}