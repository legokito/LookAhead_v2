#include "templateGen.h"
#include "visualizer.h"
#include "featureExtractor.h"

int main(int argc, char* argv[]){

	// draws the frame at each downbeat

	TemplateGen templateGen(argv[1], 24000, 96);
	FeatureExtractor featureExtractor(24000, 55, 12, 75);
	Visualizer visualizer(0);

	templateGen.loadFeatureTemplate(featureExtractor);
	int sizeOfDownbeats = templateGen.getDownbeatFrames().size();

	for (int i = 0; i < sizeOfDownbeats; i += 2){
		visualizer.drawVisualizer(
			templateGen.getFeatureTemplate().subspan(
				(int) templateGen.getDownbeatFrames()[i], 75
			)
		);
	}

	return 0;
}
