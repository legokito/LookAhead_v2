#include "hmm.h"
#include <cmath>
#include <numeric>
#include <utility>

HMM::HMM(std::span<const float> templateFrames, std::span<const int64_t> downbeatFrames, size_t K) :
	templateFrames_(templateFrames), downbeatFrames_(downbeatFrames)
{
	//templateFrames_ = templateFrames;
	//downbeatFrames_ = downbeatFrames;

	sparseTransition_ = {std::log(0.1f), std::log(0.45f), 
		std::log(0.125f), std::log(0.05f), std::log(0.0125f)}; 

	K_ = K;
	T = templateFrames_.size() / K;
	prevProbs_.assign(T, -1e30f); // log(0) = -1e30f
	prevProbs_[0] = 0.0f; // log (1) = 0, i.e. full confidence at start
	curProbs_.resize(T);

	frameNo_ = 0;
	measureNo_ = 0;
}

int HMM::updateModel(std::span<float> features_){
	// applying viterbi		
	const static float b = 10; //tuneable

	float largestProb = -1e38f;
	int largestProbIndex = 0;
	for (int i = 0; i < T; i++){
		curProbs_[i] = (b * std::inner_product(features_.begin(), 
			features_.end(), templateFrames_.data() + i*K_, 0.0));	
			
		// find largest 
		float largest = -1e38f;	
		for (int k = 0; k < 5; k++){
			if (i - k >= 0){
				float temp = prevProbs_[i - k] + sparseTransition_[k];
				if (temp > largest) largest = temp;
			}
		}

		curProbs_[i] += largest;

		if (curProbs_[i] > largestProb){
			largestProb = curProbs_[i];
			largestProbIndex = i;
		}
	}	

	for (int i = 0; i < T; i++){
		curProbs_[i] -= largestProb;
	}

	// update location
	frameNo_ = largestProbIndex;
	
	int tracker = 1;
	int trackerEnd = downbeatFrames_.size();
	while (tracker < trackerEnd){
		if (frameNo_ < downbeatFrames_[tracker-1]){
			break;
		}
		tracker += 2;
	}
	measureNo_ = downbeatFrames_[std::max(1, tracker-2)];

	std::swap(prevProbs_, curProbs_);

	return measureNo_;
}

/**
	normal formula: 
	score(i) = features_ [similarity score] frame(i) * 
				(sum of	prevProb(i...i-4) * sparseTransition(0...4))

	problem? computationally expensive.

	log formula:
	log score(i) = log of similiarty score (dot product) +
					log (
						sum of e^(
							log prevProb(i...i-4) + 
							log sparseTransition(0...4)
						)
					)
	
	problem? still computationally expensive LMAO + edge cases.
	edge cases involve accounting for log underflow below log(-87) 
	which makes everything go to 0 after a while

	viterbi (standard approximation for this use case):	
	log score(i) = b * features_ dot product frame(i) +
					max (prevProb(i...i-4) + transitionMatrix(0...4)) 	

	// prevProb and transitionMatrix already stored as logValues	
	// and ofc we normalize the new probs and swap the pointers
	
**/	


