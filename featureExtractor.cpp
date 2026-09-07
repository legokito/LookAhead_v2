#include "featureExtractor.h"
#include <span>
#include <vector>

FeatureExtractor::FeatureExtractor(double sr, double fMin, int B, size_t K) : cqt_(sr, fMin, B, K){
	features_.resize(K);
}

void FeatureExtractor::processBuffer(std::span<const float> in){
	// preprocessing

	cqt_.getMagnitudes(in, features_);

	// postprocessing
}

std::span<float> FeatureExtractor::getFeatures(){	
	return features_;
}


