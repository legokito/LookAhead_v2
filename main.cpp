#include "miniaudio.h"
#include <cstdio>
#include <iostream>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
}

int main()
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    config.playback.channels = 0;               // Set to 0 to use the device's native channel count.
    config.sampleRate        = 0;           // Set to 0 to use the device's native sample rate.
    config.dataCallback      = data_callback;   // This function will be called when miniaudio needs more data.
    config.pUserData         = NULL;   // Can be accessed from the device object (device.pUserData).

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

    ma_device_start(&device);     // The device is sleeping by default so you'll need to start it manually.

    // Do something here. Probably your program's main loop.

    ma_device_uninit(&device);
    return 0;
}
