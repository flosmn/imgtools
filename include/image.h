#pragma once

#include <vector>

struct Image
{
	Image() : 
		width(0), height(0), channels(0) 
	{}

	Image(std::size_t w, std::size_t h, std::size_t c) : 
		width(w), height(h), channels(c) 
	{
		data.resize(width*height*channels);
	}

	std::size_t width;
	std::size_t height;
	std::size_t channels;
	std::vector<float> data;

	std::size_t size() const
	{
		return width*height*channels;
	}

	std::size_t byte_size() const
	{
		return size()*sizeof(float);
	}

	void load(const char* filename);
	void save(const char* filename);
};
