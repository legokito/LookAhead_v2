#include "miniaudio.h"
#include "cqt.h"
#include "ring_buffer.h"
#include <iostream>
#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <cmath>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	// pInput is simply a memory address with sequential values    
	// pInput size = frameCount * bytesPerSample (4 for us)

	auto* rb = static_cast<RingBuffer*>(pDevice->pUserData);
	auto newSamples = static_cast<const float*>(pInput);
	std::span<const float> in(newSamples, frameCount);
	rb->addSamples(in, frameCount);
}

std::atomic<bool> isRunning(true);

int main()
{
	// variables	
	double SR = 24000;
	double fMin = 55.0; // a1
	int B = 12;
	size_t K = 75;
	CqtKernels cqt(SR, fMin, B, K);

	// must be a power of 2, otherwise we're cooked. 
	size_t buffer_size = 16384;
	RingBuffer rb(buffer_size);
	
	// sized to the max amt of frames that a pitch would need to be detected. 
	// for A1 (55hz) it is 7339 frames, which is what cqt.length(0) is. 
	std::vector<float> rb_copy(cqt.length(0)); 

	size_t hopSize = 96;
	std::vector<float> mags(K);
	

	// config
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;   
    config.capture.channels = 1;               
    config.sampleRate       = SR; // 0 for device sample rate
	config.dataCallback      = data_callback;
    config.pUserData        = &rb;

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

	
	// for aborting
	std::signal(SIGINT, [](int) { isRunning = false; });  
	std::cout << "ctrl + c to stop\n";


    //main loop
    ma_device_start(&device);     
	uint64_t prevCqtCount = rb.getTotalCount();
	while (isRunning){
		while (prevCqtCount + hopSize < rb.getTotalCount()){
			prevCqtCount += hopSize;

			// copy rb. check for tearing.
			int attempts = rb.copyRingBuffer(rb_copy, rb_copy.size()); 
			if (attempts > 0) continue;			

			// cqt			
			cqt.getMagnitudes(rb_copy, mags);

			// return frequency with most presence
			int maxFreqI = 0;	
			float maxFreq = cqt.freq(0);
			for (size_t i = 0; i < K; i++){
				if (mags[i] > mags[maxFreqI]){
					maxFreqI = i;
					maxFreq = cqt.freq(i);
				}	
			}			
			std::cout << maxFreq;
			std::cout << " - ";
			std::cout << mags[maxFreqI];
			std::cout << "\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}    
    ma_device_uninit(&device);


	std::cout << "done!\n";
    return 0;
}
