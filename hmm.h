#pragma once
#include <vector>
#include <cstdlib>
#include <cstddef>
#include <span>

// store prev  probs
// transition matrix
// store current frames
// calculate similarity score within bound 

// calculate current probs using all of the above, store it in prev probs

class HMM{	
private:
	std::vector<float> sparseTransition_;
	std::vector<float> prevProbs_;
	std::vector<float> curProbs_;
	size_t T;
	size_t K_;

	int64_t frameNo_;
	int measureNo_;

	std::span<const float> templateFrames_;
	std::span<const int64_t> downbeatFrames_; // stored as index, measure, ... 

public: 
	HMM(std::span<const float> templateFrames, std::span<const int64_t> downbeatFrames, size_t K);
	int updateModel(std::span<float> features_);	

};
