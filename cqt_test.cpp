#include "cqt.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <numbers>

int main() {
	std::vector<float> buff;
	std::vector<float> mags;

	double SR = 24000;
	size_t K = 75;

	CqtKernels kernels{SR, 55, 12, K};
	
	size_t buff_s = 8192; 

	buff.resize(buff_s);
	mags.resize(K);

	double note;

	// add a440
	note = 440.0;
	for (size_t i = 0; i < buff_s; i++){
		buff[i] += 300 * std::sin(2.0 * std::numbers::pi * note * i / SR);
	}

	// add c4	
	note = 261.63;
	for (size_t i = 0; i < buff_s; i++){
		buff[i] += 300 * std::sin(2.0 * std::numbers::pi * note * i / SR);
	}

	// add e3
	note = 164.81;
	for (size_t i = 0; i < buff_s; i++){
		buff[i] += 300 * std::sin(2.0 * std::numbers::pi * note * i / SR);
	}

	kernels.getMagnitudes(buff, mags);
		
	// print middle ones where these notes would exist. 	
	for (size_t i = 15; i < 50; i++){
		std::cout << mags[i];	
		std::cout << "\n";
	}

	return 0;
} 
