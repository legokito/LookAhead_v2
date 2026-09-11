#include "templateGen.h"
#include <vector> 
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iterator>




TemplateGen::TemplateGen(const std::filesystem::path& musicXmlPath,
		double SR, size_t hopsize){
	
	basePath = musicXmlPath;
	basePath.replace_extension();

	mxlPath = musicXmlPath;
	audioPath = basePath.string() + "audio.f32";
	downbeatsPath = basePath.string() + "downbeats.i64";

	samplingRate = SR;
	hopSize = hopsize;



	// call python file to make f32 and i64. command is blocking so we good
	std::string command = "python3 mxlToVectors.py \"" + mxlPath.string() + "\"";
	int result = std::system(command.c_str());
	if (result != 0) {
		throw std::runtime_error("mxlToVectors messed up");
	}

	// load data into vectors
	audio_ = read_vector<float>(audioPath);
	downbeats_ = read_vector<int64_t>(downbeatsPath);
}


 // do processing with feature extractor
std::span<const float> TemplateGen::getAudio() const{
	return audio_;
}

// downbeats is written as (index, measure_no., index, measure_no., ...)
std::span<const int64_t> TemplateGen::getDownbeats() const{
	return downbeats_;
}

void TemplateGen::loadFeatureTemplate(FeatureExtractor& featureExtractor){
	int blockSize = featureExtractor.cqt_.length(0);	
	int K = featureExtractor.getFeaturesSize();

	size_t iterations = (std::ssize(audio_)-blockSize) / hopSize;

	featureTemplate_.resize(iterations * K); 

	for (size_t i = 0; i < iterations; i++){
		featureExtractor.processBuffer(
			std::span(audio_).subspan(i * hopSize, blockSize),
			std::span(featureTemplate_).subspan(i * K, K)
		);

	}

	//load downbeatFrames with measure indices that correspond to featureTemplate.  
	
	downbeatFrames_ = downbeats_;
	for (size_t i = 0; i < downbeatFrames_.size(); i+=2){
		downbeatFrames_[i] = downbeats_[i] / hopSize;	
	}
}

std::span<const float> TemplateGen::getFeatureTemplate() const {
	return featureTemplate_;
}

std::span<const int64_t> TemplateGen::getDownbeatFrames() const {
	return downbeatFrames_;
}



