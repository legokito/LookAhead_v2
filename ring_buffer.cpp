#include "ring_buffer.h"
#include <vector>
#include <span>
#include <cstddef>
#include <cstdint>



RingBuffer::RingBuffer(size_t s){
	size = s;
	rb_.resize(s); 
	totalCount = 0;
}

// index 0 is the oldest sample (for in buffer)
void RingBuffer::addSamples(std::span<const float> in, size_t frameCount){
	if (frameCount > size) frameCount = size;

	uint64_t totalCountCopy = totalCount.load(std::memory_order_relaxed);
	int insertIndex = (size-1) & totalCountCopy;

	for (size_t i = 0; i < frameCount; i++){
		if (insertIndex == size) insertIndex = 0;
		rb_[insertIndex++] = in[i];
	}			
	
	totalCount.store(totalCountCopy + frameCount, std::memory_order_release);
}

// need to bound the while loop down to an exit condition otherwise can get stuck
int RingBuffer::copyRingBuffer(std::span<float> out, size_t frameCount){
	if (frameCount > size) frameCount = size;

	bool isRunning = true;
	int attempts = 0;	

	while (isRunning && attempts < 3){
		uint64_t totalCountCopy = totalCount.load(std::memory_order_acquire);
		uint64_t insertIndex = (size-1) & totalCountCopy;

		for (size_t i = frameCount; i-- > 0; ){
			if (insertIndex == 0) insertIndex = size;				
			out[i] = rb_[--insertIndex];
		}		

		// tearing policy. 
		if (totalCount - totalCountCopy < size - frameCount){
			isRunning = false;
			attempts = 0;
			break;
		}
		attempts++;
	}

	return attempts;
}

uint64_t RingBuffer::getTotalCount(){
	return totalCount.load(std::memory_order_acquire);
}
