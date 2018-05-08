#include <cstdio>
#include <image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: filename\n");
		exit(-1);
	}

	printf("Load file %s\n", argv[1]);

	int w, h, n;
	unsigned char* data = stbi_load(argv[1], &w, &h, &n, 0);

	printf("w %d, h %d, n %d\n", w, h, n);

	printf("write image as test.jpg\n");
	stbi_write_jpg("test.jpg", w, h, n, data, 100);

	stbi_image_free(data);

	return 0;
}
