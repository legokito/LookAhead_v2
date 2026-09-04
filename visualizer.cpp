#include "visualizer.h"
#include <iostream>
#include <cstddef>
#include <span>
#include <vector>
#include <algorithm>

Visualizer::Visualizer(int count){
	asciiCounter = count;
}

void Visualizer::drawVisualizer(std::span<const float> mags){
             if (asciiCounter % 30 == 0){
                 for (int i = 1; i < 8; i++){
                     std::cout << "---C--------";
                 }
                 std::cout << "\n";
             }
 
                 for (int i = 0; i < mags.size(); i++){
                     int idx = std::min(static_cast<int>(mags[i] / 0.0005f), 4);
                     char asciiChar = asciiDict[idx];
                     std::cout << asciiChar;
                 }
                 std::cout << "\n";
 
			asciiCounter++;
}
