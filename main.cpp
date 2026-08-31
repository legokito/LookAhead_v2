#include "miniaudio.h"
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

	
}




std::atomic<bool> isRunning(true);

int main()
{
	// variables	
	
	

	// config
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;   
    config.capture.channels = 1;               
    config.sampleRate       = 24000; // 0 for device sample rate
    config.dataCallback     = data_callback; // refill buffer
	config.periodSizeInFrames = 256; //suggestion, line 6877 in .h
    // config.pUserData        = ;

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

	
	// for aborting
	std::signal(SIGINT, [](int) { isRunning = false; });  
	std::cout << "ctrl + c to stop\n";


    //main loop
    ma_device_start(&device);     
	while (isRunning){
		

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}    
    ma_device_uninit(&device);


	std::cout << "done!\n";
    return 0;
}
