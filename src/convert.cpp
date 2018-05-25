#include <cstdio>
#include <vector>
#include <cstring>
#include <cmath>

#include <image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("Usage: convert input output [--gamma] [--exposure]\n");
		exit(-1);
	}
	
	float gamma = 1.f;
	float exposure = 0.f;

	if (argc > 3) 
	{
		if ((argc - 3) % 2 != 0)
		{
			printf("Usage: convert input output [--gamma] [--exposure]\n");
			exit(-1);
		}

		for (int arg = 3; arg < argc; arg += 2)
		{
			if (std::strcmp(argv[arg], "--gamma") == 0) {
				gamma = std::atof(argv[arg+1]);
			}
			if (std::strcmp(argv[arg], "--exposure") == 0) {
				exposure = std::atof(argv[arg+1]);
			}
		}
	}

	printf("convert %s to %s using gamma %f and exposure %f\n", 
			argv[1], argv[2], gamma, exposure);

	Image src;
	src.load(argv[1]);
	
	Image dst(src.width, src.height, src.channels);
	// ignore alpha channel
	for (std::size_t y = 0; y < src.height; ++y)
	for (std::size_t x = 0; x < src.width;  ++x)
	{
		std::size_t offset = src.channels * (src.width * y + x);
		for (int c = 0; c < 3; ++c)
		{
			float v = std::pow(2, exposure) * src.data[offset + c];
			if (gamma > 0.f)
				v = pow(v, 1.f/gamma);
			dst.data[offset + c] = v;
		}
		if (src.channels == 4)
			dst.data[offset + 3] = src.data[offset + 3];
	}

	dst.save(argv[2]);

	return 0;
}
