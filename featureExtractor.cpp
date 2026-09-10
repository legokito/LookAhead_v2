#include "featureExtractor.h"
#include <span>
#include <vector>

FeatureExtractor::FeatureExtractor(double sr, double fMin, int B, size_t K) : cqt_(sr, fMin, B, K){
	features_.resize(K);
}

void FeatureExtractor::processBuffer(std::span<const float> in, std::span<float> out){
	// preprocessing

	cqt_.getMagnitudes(in, out);

	// postprocessing
}

std::span<const float> FeatureExtractor::getFeatures(){	
	return features_;
}

size_t FeatureExtractor::getFeaturesSize(){
	return features_.size();
}
