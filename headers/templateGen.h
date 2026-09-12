#pragma once
#include <filesystem>
#include <fstream>
#include <cstddef>
#include <span>
#include <vector>
#include "featureExtractor.h"

class TemplateGen{
private:
	std::filesystem::path basePath;
	std::filesystem::path mxlPath;
	std::filesystem::path audioPath;
	std::filesystem::path downbeatsPath;
 
	double samplingRate;
	size_t hopSize;
 
	std::vector<float> audio_;
	std::vector<int64_t> downbeats_;
	
	std::vector<float> featureTemplate_;
	std::vector<int64_t> downbeatFrames_;

	template <typename T>
	std::vector<T> read_vector(const std::filesystem::path& path){
		std::ifstream file(path, std::ios::binary);
		const auto size = std::filesystem::file_size(path);
	
		std::vector<T> data(size / sizeof(T));
		
		file.read(reinterpret_cast<char*>(data.data()),
					static_cast<std::streamsize>(size));

		return data;
	}

public:
	TemplateGen (
		const std::filesystem::path& musicXmlPath,
		double SR, size_t hopsize
	);
	std::span<const float> getAudio() const;
	std::span<const int64_t> getDownbeats() const;

	void loadFeatureTemplate(FeatureExtractor& featureExtractor);
	std::span<const float> getFeatureTemplate() const;
	std::span<const int64_t> getDownbeatFrames() const;

};

