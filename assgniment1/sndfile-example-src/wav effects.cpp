#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.hh>

int main(int argc, char *argv[]){

    // check number of args
    if(argc < 4){
        cerr << "Usage: " << argv[0] << " <input file> <output file> <effect> <fequency | delay> <gain>";
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
        std::cerr << "Error creating output file.";
        return 1;
    }
    
    effect = argv[3];
    if (effect != "single_echo" || effect != "mult_echo" || effect != "amp_mod" || effect != "time_delay"){
        cerr << "Invalid Effect!\nPossible effects: single_echo, mult_echo, amp_mod, time_delay!";
        return 1;
    }

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * inputFile.channels());
    short sampleOut;
    //  A single echo
    if (effect == "single_echo" || effect == "mult_echo"){
        
        try{
            int delay = std::atoi(argv[argc-2]);
            float gain = std::atoi(argv[argc-1]);
        } catch{
            cerr << "Invalid arguments!";
            return 1;
        }

        if (effect == "single_echo"){
            while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
                samples.resize(nFrames * inputFile.channels());
                for (int i = 0; i < (int)samples.size(); i++){
                    if (i >= delay){
                        sampleOut = (samples.at(i) + gain * samples.at(i-delay)) / (1 + gain);
                    }
                }
            }
        } else if (effect == "mult_echo"){
            while((nFrames = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
                samples.resize(nFrames * inputFile.channels());
                for (int i = 0; i < (int)samples.size(); i++){
                    if (i >= delay){
                        sampleOut = (samples.at(i) + gain * samplesOut.at(i - delay)) / (1 + gain);
                    }
                }
            }
        }

        samplesOut.insert(samplesOut.end(), sampleOut);

    } 
    //  Amplitude modulation
    else if (effect == "amp_mod") {
        
        try{
            float gainn = std::atoi(argv[argc-1]);
        } catch{
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
    

}