#pragma once
#include <iostream>
#include <span>
#include <cstddef>
#include <vector>

class Visualizer {
private:
	int asciiCounter;
	static constexpr std::array<char, 5> asciiDict = {' ', '.', '*', '#', '@'}; // same value across instances. 

public: 
	Visualizer(int count);
	void drawVisualizer(std::span<const float> mags);
};

