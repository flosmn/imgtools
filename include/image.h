#pragma once

#include <vector>

struct Image
{
	Image() : 
		width(0), height(0), channels(0) 
	{}

	Image(size_t w, size_t h, size_t c) : 
		width(w), height(h), channels(c) 
	{
		data.resize(width*height*channels);
	}

	size_t width;
	size_t height;
	size_t channels;
	std::vector<float> data;

	size_t size() const
	{
		return width*height*channels;
	}

	size_t byte_size() const
	{
		return size()*sizeof(float);
	}

	void load(const char* filename);
	void save(const char* filename);
};
