#pragma once
#include <vector>
#include <span>
#include <cstddef>
#include <atomic>
#include <cstdint>

class RingBuffer {
private:
	std::vector<float> rb_;
	size_t size;
	std::atomic<uint64_t> totalCount;

public:
	RingBuffer(size_t s);
	void addSamples(std::span<const float> in, size_t frameCount);
	int copyRingBuffer(std::span<float> out, size_t frameCount);
	uint64_t getTotalCount();
};
