#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: filename\n");
		return 0;
	}

	printf("Load file %s\n", argv[1]);

	int w, h, n;
	unsigned char* data = stbi_load(argv[1], &w, &h, &n, 0);

	printf("w %d, h %d, n %d\n", w, h, n);
	stbi_image_free(data);

	return 0;
}