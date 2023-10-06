#ifndef WAVEFFECTS_H
#define WAVEFFECTS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <sndfile.hh>

using namespace std;

class WAVEffects {
  private:
	std::vector<short> samples;
	std::vector<short> created;

  public:
	WAVEffects() {}

	void update(const std::vector<short>& samples) {		
		for (short entry: samples) {
			this->original.insert(this->original.end(), entry);
		}
	}

	void apply(int effect, char* parameters[]) {
		switch (effect) {
			case 0: // echo
				int repetitions = stoi(parameters[0]);
				int delta = stoi(parameters[1]);
				double proportion = stoi(parameters[2])/100;
				created.resize(original.size() + delta*repetitions);
				for (int i = 0; i < original.size(); i++) {
					for (int j = 0; j <= repetitions; j++) {
						created[i + j*delta] += original[i]*pow(proportion, j);
					}
				}
				break;
			default:
				break;
		original = created;
		created.resize(0);
		}
	}

	void toFile(SndfileHandle sfhOut) const{
        sfhOut.write(original.data(), original.size());
    }
};

#endif
