#pragma once
#include "cqt.h"
#include <vector>
#include <cstddef>
#include <span>


class FeatureExtractor{
private: 
	std::vector<float> features_;
	std::vector<float> in_;

public:
	CqtKernels cqt_;
	FeatureExtractor(double sr, double fMin, int B, size_t K);
	
	void processBuffer(std::span<const float> in, std::span<float> out);
	void processBuffer(std::span<const float> in) { 
			processBuffer(in, features_); };

	std::span<const float> getFeatures();
	size_t getFeaturesSize();
};
