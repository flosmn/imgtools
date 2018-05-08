#include <image.h>

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>

void Image::
load(const char* filename)
{
	float* src;
	const char* err;
	int w, h;
	int ret = LoadEXR(&src, &w, &h, filename, &err);
	if (ret != TINYEXR_SUCCESS) {
		printf("error loading exr %s: %s\n", filename, err);
		return;
	}
	width = w;
	height = h;
	channels = 4;
	data.resize(size());
	memcpy(data.data(), src, byte_size());
}

void Image::
save(const char* filename)
{
	assert(channels == 4);
	int ret = SaveEXR(data.data(), width, height, channels, /*fp16*/0, filename);
	if (ret != 0)
	{
		printf("error writing exr %s: %d\n", filename, ret);
		return;
	}
}

