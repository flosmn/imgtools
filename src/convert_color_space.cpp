#include <cstdio>
#include <vector>
#include <cstring>
#include <cmath>

#include <image.h>

static float to_XYZ[9] = {
0.4124564, 0.3575761, 0.1804375,
0.2126729, 0.7151522, 0.0721750,
0.0193339, 0.1191920, 0.9503041,
};

static float from_XYZ[9] = {
 3.2404542, -1.5371385, -0.4985314,
-0.9692660,  1.8760108,  0.0415560,
 0.0556434, -0.2040259,  1.0572252
};

static inline void transform(
	const float M[9],
	const float a[3],
	float res[3])
{
	res[0] = M[0] * a[0] + M[1] * a[1] + M[2] * a[2];
	res[1] = M[3] * a[0] + M[4] * a[1] + M[5] * a[2];
	res[2] = M[6] * a[0] + M[7] * a[1] + M[8] * a[2];
}

int main(int argc, char** argv)
{
	// asumes an image in srgb space and performes a bradford adaptation to illuminant E
	if (argc < 4) {
		printf("Usage: convert_color_space from to file. form and to can be 'rgb' or 'xyz'.\n");
		exit(-1);
	}

	Image src;
	src.load(argv[3]);

	int convert_to_XYZ = -1;
	if (std::strcmp(argv[1], "xyz") == 0 && std::strcmp(argv[2], "rgb") == 0) {
		convert_to_XYZ = 0;
	}
	if (std::strcmp(argv[1], "rgb") == 0 && std::strcmp(argv[2], "xyz") == 0) {
		convert_to_XYZ = 1;
	}

	if (convert_to_XYZ == 1)
		printf("convert rgb to xyz\n");
	else if (convert_to_XYZ == 0)
		printf("convert xyz to rgb\n");
	else
		fprintf(stderr, "unrecognized conversion\n");
	
	for (std::size_t y = 0; y < src.height; ++y)
	for (std::size_t x = 0; x < src.width;  ++x)
	{
		std::size_t offset = src.channels * (src.width * y + x);

		float csrc[3] = {
			src.data[offset + 0],
			src.data[offset + 1],
			src.data[offset + 2]
		};

		float cdst[3] = {0};
		if (convert_to_XYZ == 0)
			transform(from_XYZ, csrc, cdst);
		else if (convert_to_XYZ == 1)
			transform(to_XYZ, csrc, cdst);
		else {
			cdst[0] = csrc[0];
			cdst[1] = csrc[1];
			cdst[2] = csrc[2];
		}

		src.data[offset + 0] = cdst[0];
		src.data[offset + 1] = cdst[1];
		src.data[offset + 2] = cdst[2];
	}

	src.save(argv[3]);

	return 0;
}
