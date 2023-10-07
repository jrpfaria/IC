#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.hh>
#include "wav_effects.h"

using namespace std;
constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]){

    // check number of args
    if(argc < 4){
        cerr << "Usage: " << argv[0] << " <input file> <output file> <effect> <parameters>";
        return 1;
    }

    SndfileHandle inputFile { argv[1] };
    // check if correct input file
    if (inputFile.error()){
        cerr << "Invalid input file!\n";
        return 1;
    }

    SndfileHandle outputFile { argv[2], SFM_WRITE, inputFile.format(), inputFile.channels(), inputFile.samplerate() };
    if (not outputFile){
        cerr << "Error creating output file.";
        return 1;
    }
    
    int effect;
    try {
        effect = stoi(argv[3]);
    }
    catch (const exception &) {
        cerr << "Invalid effect";
        return 1;
    }
    char* parameters[argc-4];
    std::copy(argv+4, argv+argc, parameters);

    vector<short> audioSamples(inputFile.frames());
    inputFile.readf(audioSamples.data(), audioSamples.size());

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * inputFile.channels());
    WAVEffects effects { inputFile };
    while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * inputFile.channels());
		effects.update(samples);
	}
    effects.apply(effect, parameters);
    effects.toFile(outputFile);
    
    return 0;
    /*
    int delay;
    float gain;
    //  A single echo
    if (effect == "single_echo" || effect == "mult_echo"){
        
        try{
            delay = std::atoi(argv[argc-2]);
            gain = std::atof(argv[argc-1]);
        } catch(exception &err){
            cerr << "Invalid arguments!";
            return 1;
        }

        if (effect == "single_echo"){
            while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
                samples.resize(nFrames * inputFile.channels());
                for (int i = 0; i < (int)samples.size(); i++){
                    if (i >= delay){
                        sampleOut = (samples.at(i) + gain * samples.at(i-delay)) / (1 + gain);
                    }else{sampleOut = samples.at(i);}
                }
            }
        } else if (effect == "mult_echo"){
            while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
                samples.resize(nFrames * inputFile.channels());
                for (int i = 0; i < (int)samples.size(); i++){
                    if (i >= delay){
                        sampleOut = (samples.at(i) + gain * samplesOut.at(i - delay)) / (1 + gain);
                    }else{sampleOut = samples.at(i);}
                }
            }
        }

        samplesOut.insert(samplesOut.end(), sampleOut);

    } 
    //  Amplitude modulation
    else if (effect == "amp_mod") {
        
        float gainn;

        try{
            gainn = std::atoi(argv[argc-1]);
        } catch(exception &err){
            cerr << "Invalid arguments!";
            return 1;
        }

        while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
            samples.resize(nFrames * inputFile.channels());
            
        }
    
    } 
    //  Time-varying delays
    else if (effect == "time_delay"){
        
        while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
            samples.resize(nFrames * inputFile.channels());
            
        }
    
    }
    */

}