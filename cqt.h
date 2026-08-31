#pragma once
#include <vector>
#include <span>
#include <cstddef>

class CqtKernels {
private:
	std::vector<float> data_;
	std::vector<size_t> offsets_;
	std::vector<size_t> lengths_;
	std::vector<double> freqs_;

	size_t K; 		// # of bins
	int B;			// binsPerOctave
	double SR; 		// sampleRate
	double FMIN;	// lowest freq bin
	double Q; 		// Q value for CQT

public: 
	CqtKernels(double sampleRate, double fMin, int binsPerOctave, size_t numBins);
	
	size_t numBins() const;
	size_t length(size_t k) const; //# of frames. 					k = bin index. 
	double freq(size_t k) const; //freq in hz. 						k = bin index.
	std::span<const float> kernel(size_t k) const; //kernel values.	k = bin index.
	void getMagnitudes(std::span<const float> in, std::span<float> out) const;

};

