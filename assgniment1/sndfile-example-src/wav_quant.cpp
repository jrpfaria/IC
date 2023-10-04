#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

int main(int argc, char *argv[]){
    
    // check number of args
    if(argc < 4){
        cerr << "Usage: " << argv[0] << " <input file> <output file> <quantization level>" << endl;
        return -1;
    }

    // get input
    SndfileHandle inputFile { argv[argc-3] };
    // check if correct input file
    if (inputFile.error()){
        cerr << "Invalid input file!\n";
        return -1;
    }

    const char* outputFilename= argv[argc-2];

    SndfileHandle outputFile {outputFilename, SFM_WRITE, inputFile.format(), inputFile.channels(), inputFile.samplerate()};
    if (not outputFile) return -1;

    int btc = std::atoi(argv[argc-1]);
    if (btc < 1 || btc > 16){
        cerr << "Invalid bits to keep!";
        return 1;
    }

    // supostamente temos de fazer isto antes de poder usar na quant ainnda nao percebi pq
    vector<short> audioSamples(inputFile.frames());
    inputFile.readf(audioSamples.data(), audioSamples.size());

    WAVQuant wavQuant;
    wavQuant.quant(audioSamples, btc);
    wavQuant.toFile(outputFile);
}