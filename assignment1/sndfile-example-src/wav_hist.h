#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::size_t no_channel;
	int bin_size;

  public:
	WAVHist(const SndfileHandle& sfh, int a_bin_size) {
		no_channel = sfh.channels();
		counts.resize(sfh.channels());
		bin_size = a_bin_size;

		// insert new maps for SIDE and MID channels
		if(sfh.channels() == 2)
			for(int i = 0; i < 2; i++) {
				counts.push_back(std::map<short, size_t>());
			}
	}

	void update(const std::vector<short>& samples) {		
		for(int i = 0; i < int(samples.size()); i += 2){
			short left_channel = samples[i];
			short right_channel = samples[i+1];

			// LEFT channel
			counts[0][int(left_channel/bin_size)*bin_size]++;

			// RIGHT channel 
			counts[1][int(right_channel/bin_size)*bin_size]++;

			// MID channel (L + R) / 2
			counts[2][(int((left_channel + right_channel) / 2)/bin_size)*bin_size]++;
			
			// SIDE channel (L - R) / 2
			counts[3][(int((left_channel - right_channel) / 2)/bin_size)*bin_size]++;
		}
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}
};

#endif

