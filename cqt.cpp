#include "cqt.h"
#include <vector>
#include <cmath>
#include <numbers>

CqtKernels::CqtKernels(double sampleRate, double fMin, int binsPerOctave, size_t numBins)
							: SR(sampleRate), FMIN(fMin), B(binsPerOctave), K(numBins) 
{
	Q = 1 / (std::pow(2, 1.0/binsPerOctave) - 1);
	double PI = std::numbers::pi;
	size_t SUM_LENGTHS = 0;

	freqs_.resize(K);
	lengths_.resize(K);
	offsets_.resize(K+1);
	
	// helper data	
	offsets_[0] = 0;
	for (size_t k = 0; k < K; k++){
		freqs_[k] = freq(k);
		lengths_[k] = length(k);  	
		offsets_[k+1] = 2 * length(k) + offsets_[k]; // 2x for cos and sin interleaving
		SUM_LENGTHS += lengths_[k];
	}	

	data_.resize(2 * SUM_LENGTHS);

	// kernels computation
	for (size_t k = 0; k < K; k++){

		// hann window 
		for (size_t i = 0; i < lengths_[k]; i++){
			data_[2*i + offsets_[k]] = 0.5 * (1 - std::cos(2*PI*i / (lengths_[k] - 1)));
			data_[2*i + 1 + offsets_[k]] = 0.5 * (1 - std::cos(2*PI*i / (lengths_[k] - 1)));
		}	 

		// kernel values
		for (size_t i = 0; i < lengths_[k]; i++){
			data_[2*i + offsets_[k]] *= std::cos(2*PI*Q*i / lengths_[k]);
			data_[2*i + 1 + offsets_[k]] *= -1 * std::sin(2*PI*Q*i / lengths_[k]);
	
		}
	}
}


size_t CqtKernels::numBins() const {
	return K;
}

double CqtKernels::freq(size_t k) const {
	return FMIN * std::pow(2, static_cast<double>(k)/B);
}

size_t CqtKernels::length(size_t k) const {
	return std::ceil(Q * SR / freq(k));
}

std::span<const float> CqtKernels::kernel(size_t k) const{
	return {data_.data() + offsets_[k], 2 * lengths_[k]};
}

void CqtKernels::getMagnitudes(
	std::span<const float> in, std::span<float> out) const
{
	size_t s = in.size();

	for (size_t k = 0; k < K; k++){
		float re = 0.0;
		float im = 0.0;
		for (size_t l = 0; l < lengths_[k]; l++){
			im += in[s - 1 - l] * data_[offsets_[k+1] - 1 - 2*l];
			re += in[s - 1 - l] * data_[offsets_[k+1] - 1 - 2*l - 1];
		}			
		out[k] = std::sqrt(re*re + im*im) / lengths_[k];
	}
	
}

