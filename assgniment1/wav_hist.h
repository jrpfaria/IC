#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
		bin_sizes.resize(sfh.channels(), bin_size);

        if (sfh.channels() == 2) {
			// Initialize an empty map for MID and SIDE channels
            histograms.push_back(std::map<int, size_t>()); 
            histograms.push_back(std::map<int, size_t>());

			// Push back bin sizes for MID and SIDE channels
            bin_sizes.push_back(bin_size);
            bin_sizes.push_back(bin_size);
        }
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}
};

#endif

