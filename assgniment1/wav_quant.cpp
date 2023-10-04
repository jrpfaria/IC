#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"
#include <string.h>
#include <math.h>

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 4){
        cerr << "Usage: " << argv[0] << " <input file> <output file> <quantization level>" << endl;
        return -1;
    }

    SndfileHandle sfhIn {argv[1]};
    if(sfhIn.error()){
        cerr << "Error: invalid input file" << endl;
        return -1;
    }

    SndfileHandle sfhOut {argv[2], SFM_WRITE, sfhIn.format(), sfhIn.channels(), sfhIn.samplerate()};
    if(sfhOut.error()){
        cerr << "Error: invalid output file" << endl;
        return -1;
    }

    WAVQuant wavQuant;
    wavQuant.quant(sfhIn, atoi(argv[3]));
    wavQuant.toFile(sfhOut);

    return 0;
}