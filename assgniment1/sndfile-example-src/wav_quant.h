#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVQuant{
    private: 
        std::vector<short> quant_samples;
        int btc;

    public:
        WAVQuant(int quant){
            btc = quant;
        }

        void update(const std::vector<short>& samples) {
            for(short entry: samples){
                entry >>= btc;
                entry <<= btc;
                quant_samples.insert(quant_samples.end(), entry);
            }
        }

        void toFile(SndfileHandle sfhOut) const{
            sfhOut.write(quant_samples.data(), quant_samples.size());
        }
};

#endif
