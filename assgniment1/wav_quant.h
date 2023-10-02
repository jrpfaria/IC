#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVQuant{
    private: 
        std::vector<short> quant_samples;

    public:
        WAVQuant()
            quant_samples.resize(0);

        void quant(const std::vector<short>& samples, size_t quant) {
            for(auto entry : samples){
                entry = samples >> quant;
                quant_samples.insert(quant_samples.end(), entry << quant);
            }
        }

        void toFile(SndfileHandle sfhOut) const{
            sfhOut.write(quant_samples.data(), quant_samples.size());
        }
}

#endif