#include <cstdio>
#include <vector>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>

int main(int argc, char** argv)
{
	if (argc < 5) {
		printf("Usage: resize input.exr output.exr dst_width dst_height\n");
		exit(-1);
	}

	int dst_width = atoi(argv[3]);
	int dst_height = atoi(argv[4]);

	int src_width, src_height;
	float* src;
	const char* err;

	int ret = LoadEXR(&src, &src_width, &src_height, argv[1], &err);
	if (ret != TINYEXR_SUCCESS) {
		printf("error loading exr %s: %s\n", argv[1], err);
		return -1;
	}

	std::vector<float> dst(dst_width * dst_height * 4);
	ret = stbir_resize_float(src, src_width, src_height, 0,
			dst.data(), dst_width, dst_height, 0, 4);
	if (ret != 1)
	{
		printf("error resizing exr %s: %d\n", argv[1], ret);
		return -1;
	}
	
	ret = SaveEXR(dst.data(), dst_width, dst_height, 4, /*fp16*/0, argv[2]);
	if (ret != 0)
	{
		printf("error writing exr %s: %d\n", argv[2], ret);
		return -1;
	}

	return 0;
}
