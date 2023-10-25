#ifndef WAVEFFECTS_H
#define WAVEFFECTS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <sndfile.hh>

using namespace std;

class WAVEffects {
  private:
	int channels;
	int samplerate;
	std::vector<short> original;
	std::vector<short> created;

  public:
	WAVEffects(const SndfileHandle& sfh) {
		this->channels = sfh.channels();
		this->samplerate = sfh.samplerate();
	}

	void update(const std::vector<short>& samples) {		
		for (short entry: samples) {
			this->original.insert(this->original.end(), entry);
		}
	}

	void apply(int effect, char* parameters[]) {
		switch (effect) {
			case 0: // repetition
			{	
				int repetitions = stoi(parameters[0]);
				int delta = stod(parameters[1])*samplerate*channels;
				double proportion = stod(parameters[2])/100;
				created.resize(original.size() + repetitions*delta);
				for (int i = 0; i < int(original.size()); i++) {
					for (int j = 0; j <= repetitions; j++) {
						created[i + j*delta] += short(original[i]*pow(proportion, j));
					}
				}
				created.resize(original.size());
				break;
			}
			case 1: // multiply amplitude
			{
				double proportion = stod(parameters[0])/100;
				created.resize(original.size());
				for (int i = 0; i < int(original.size()); i++) {
					created[i] = short(original[i]*proportion);
				}
				break;
			}
			default:
				break;
		}
	}

	void toFile(SndfileHandle sfhOut) const{
        sfhOut.write(created.data(), created.size());
    }
};

#endif
