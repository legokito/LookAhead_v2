#include "miniaudio.h"
#include "visualizer.h"
#include "cqt.h"
#include "ring_buffer.h"
#include "featureExtractor.h"
#include "templateGen.h"
#include "hmm.h"
#include <iostream>
#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <cmath>

std::atomic<bool> isRunning(true);

struct Ctx {
      RingBuffer* rb;
      ma_decoder* dec;
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
      // playback mode: pInput is NULL. pull from decoder into pOutput,
      // then copy that same data into the ring buffer for analysis.
      (void)pInput;

      auto* ctx = static_cast<Ctx*>(pDevice->pUserData);
      auto* out = static_cast<float*>(pOutput);

      ma_uint64 read = 0;
      ma_decoder_read_pcm_frames(ctx->dec, out, frameCount, &read);

      for (ma_uint64 i = read; i < frameCount; i++) out[i] = 0.0f;

      ctx->rb->addSamples({out, static_cast<size_t>(read)}, static_cast<size_t>(read));

      if (read < frameCount) isRunning = false;   // EOF
}


int main(int args, char* argv[])
{

	// variables	
	double SR = 24000;
	double fMin = 55.0; // a1
	int B = 12;
	size_t K = 75;
	size_t hopSize = 96;
	size_t buffer_size = 16384;

	FeatureExtractor featureExtractor(SR, fMin, B, K);
	TemplateGen templateGen(argv[1], SR, hopSize);
	templateGen.loadFeatureTemplate(featureExtractor);
	HMM hmm(templateGen.getFeatureTemplate(), 
		templateGen.getDownbeatFrames(), K);


	// must be a power of 2, otherwise we're cooked. 
	RingBuffer rb(buffer_size);
	
	// sized for A1 (55hz) it is 7339 frames, 
	// which is what cqt.length(0) is. 
	// this is to match cqt architecture since the copy
	// gets passed to be analyzed
	std::vector<float> rb_copy(featureExtractor.cqt_.length(0)); 
	std::vector<float> features_(K);



	// decoder: resamples/downmixes whatever the wav is into 24kHz mono f32
	ma_decoder_config dcfg = ma_decoder_config_init(ma_format_f32, 1, 24000);
	ma_decoder decoder;
	if (ma_decoder_init_file(argv[2], &dcfg, &decoder) != MA_SUCCESS) {
			std::cerr << "cannot open wav: " << argv[2] << "\n";
			return 1;
	}

	Ctx ctx{&rb, &decoder};

	// playback, because audio comes from the file not the mic
	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.format   = ma_format_f32;
	config.playback.channels = 1;
	config.sampleRate        = static_cast<ma_uint32>(SR);
	config.dataCallback      = data_callback;
	config.pUserData         = &ctx;

	ma_device device;
	if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
			ma_decoder_uninit(&decoder);
			std::cerr << "failed to init device\n";
			return 1;
	}


	// for aborting
	std::signal(SIGINT, [](int) { isRunning = false; });  
	std::cout << "ctrl + c to stop\n";

    //main loop
    ma_device_start(&device);     
	uint64_t prevCqtCount = rb.getTotalCount();

	int measureNo_ = 1;

	while (isRunning){
		while (prevCqtCount + hopSize < rb.getTotalCount()){

			// copy rb. check for tearing.
			int attempts = rb.copyRingBuffer(rb_copy, rb_copy.size()); 
			if (attempts > 0) continue;			

			prevCqtCount += hopSize;
			// cqt			
			featureExtractor.processBuffer(rb_copy, features_);
			measureNo_ = hmm.updateModel(features_);
			std::cout << measureNo_ << std::endl;

			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}    
    ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);


	std::cout << "done!\n";
    return 0;
}
