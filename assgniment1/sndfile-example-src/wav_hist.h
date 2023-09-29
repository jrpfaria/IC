#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::size_t bin_sizes; // up to 2^16
	std::size_t no_channel;

  public:
	WAVHist(const SndfileHandle& sfh, size_t bin_size) {
		no_channel = sfh.channels();
		counts.resize(sfh.channels());
		bin_sizes = bin_size;

		// insert new maps for SIDE and MID channels and set bin sizes for both
		if(sfh.channels() == 2)
			for(int i = 0; i < 2; i++) {
				counts.push_back(std::map<short, size_t>()); 
				bin_sizes.push_back(bin_size);
			}
	}

	void update(const std::vector<short>& samples) {		
		for(int i = 0; i < samples.size(); i += 2){
			// -2^15 - 2^15 / 2^k = valor
			// Fix to desired bin size
			short left_channel = samples[i]; // divisão inteira por valor
			short right_channel = samples[i+1];

			// LEFT channel
			counts[0][left_channel]++;

			// RIGHT channel 
			counts[1][right_channel]++;

			// MID channel (L + R) / 2
			counts[2][(left_channel + right_channel) / 2]++;
			
			// SIDE channel (L - R) / 2
			counts[3][(left_channel - right_channel) / 2]++;
		}

		//bin sizes
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}
};

#endif

