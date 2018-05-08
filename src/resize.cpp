#include <cstdio>
#include <vector>
#include <cstring>

#include <image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

int main(int argc, char** argv)
{
	if (argc < 5) {
		printf("Usage: resize input.exr output.exr dst_width dst_height\n");
		exit(-1);
	}

	Image src;
	src.load(argv[1]);

	int dst_width = atoi(argv[3]);
	int dst_height = atoi(argv[4]);
	Image dst(dst_width, dst_height, 4);

	int ret = stbir_resize_float(src.data.data(), src.width, src.height, 0,
			dst.data.data(), dst.width, dst.height, 0, 4);
	if (ret != 1)
	{
		printf("error resizing exr %s: %d\n", argv[1], ret);
		return -1;
	}
	
	dst.save(argv[2]);

	return 0;
}
