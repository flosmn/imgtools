#include <cstdio>
#include <vector>
#include <cstring>
#include <cmath>

#include <image.h>

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: mean_image_brightness input\n");
		exit(-1);
	}

	Image src;
	src.load(argv[1]);
	
	double brightness = 0;
	double brightness_rgb[3] = {0};
	for (std::size_t y = 0; y < src.height; ++y)
	for (std::size_t x = 0; x < src.width;  ++x)
	{
		std::size_t offset = src.channels * (src.width * y + x);
		double bpx = 0;
		for (int c = 0; c < 3; ++c)
		{
			bpx += src.data[offset + c];
			brightness_rgb[c] += src.data[offset + c];
		}
		bpx /= 3.0;
		brightness += bpx;
	}
	for (int c = 0; c < 3; ++c) {
		brightness_rgb[c] /= (src.height * src.width);
	}
	brightness /= (src.height * src.width);
	printf("mean image brightness (%f, %f, %f), (%f)\n", 
			brightness_rgb[0], brightness_rgb[1], brightness_rgb[2],
			brightness);
	return 0;
}
