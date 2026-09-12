#include "headers/featureExtractor.h"
#include <span>
#include <vector>
#include <algorithm>

FeatureExtractor::FeatureExtractor(double sr, double fMin, int B, size_t K) : cqt_(sr, fMin, B, K){
	features_.resize(K);
}

void FeatureExtractor::processBuffer(std::span<const float> in, std::span<float> out){
	
	out_.resize(out.size());

	// cqt
	cqt_.getMagnitudes(in, out_);

	// l2 norm
	float sum = 0;
	for (int i = 0; i < out.size(); i++){
		out[i] = out_[i];
		sum += out_[i] * out_[i];
	}
	
	float sqrt = std::sqrt(sum);
	for (int i = 0; i < out.size(); i++){
		if (sqrt == 0) out[i] = 0;
		else out[i] /= sqrt;
	}


	// postprocessing - make out_ for it
}

std::span<const float> FeatureExtractor::getFeatures(){	
	return features_;
}

size_t FeatureExtractor::getFeaturesSize(){
	return features_.size();
}
