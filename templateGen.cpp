#include "templateGen.h"
#include <filesystem>

TemplateGen::TemplateGen(const std::filesystem::path& musicXmlPath,
		double SR, size_t mCS, size_t hopsize){
	
	basePath = musicXMLPath;
	basePath.replace_extension();

	mxlPath = musicXMLPath;
	audioPath = base.string() + "audio.f32";
	downbeatsPath = base.string() + "downbeats.i64";

	samplingRate = SR;
	maxCqtSize = mCS;
	hopSize = hopsize;



	// call python file to make f32 and i64. command is blocking so we good
	std::string command = "python3 mxlToVectors.py \"" + mxlPath.string() + "\"";
	int result = std::system(command.c_str());
	if (result != 0) {
		throw std::runtime_error("mxlToVectors messed up");
	}

	// load data into vectors
	audio_ = read_vector(audioPath);
	downbeats_ = read_vector(downbeatsPath);
}


 // do processing with feature extractor
std::span<const float> TemplateGen::getAudio() const{
	return audio_;
}

// downbeats is written as (index, measure_no., index, measure_no., ...)
std::span<const float> TemplateGen::getDownbeats() const{
	return downbeats_;
}

void TemplateGen::loadFeatureTemplate(FeatureExtractor featureExtractor){
		

}

std::span<std::span<const float>> TemplateGen::getFeatureTemplate() const{
	return featureTemplate_;
}





