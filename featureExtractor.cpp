#include "featureExtractor.h"
#include <span>
#include <vector>
#include <algorithm>

FeatureExtractor::FeatureExtractor(double sr, double fMin, int B, size_t K) : cqt_(sr, fMin, B, K){
	features_.resize(K);
}

void FeatureExtractor::processBuffer(std::span<const float> in, std::span<float> out){
	in_.resize(in.size());
	
	// l2 norm
	float sum = 0;
	for (int i = 0; i < in.size(); i++){
		in_[i] = in[i];
		sum += in[i] * in[i];
	}
	
	float sqrt = std::sqrt(sum);
	for (int i = 0; i < in.size(); i++){
		in_[i] /= sqrt;
	}

	// cqt
	cqt_.getMagnitudes(in_, out);

	// postprocessing - make out_ for it
}

std::span<const float> FeatureExtractor::getFeatures(){	
	return features_;
}

size_t FeatureExtractor::getFeaturesSize(){
	return features_.size();
}
