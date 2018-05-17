#include <cstdio>
#include <vector>
#include <cstring>
#include <cmath>

#include <image.h>

static float B[9] = {
// 1.0502616,  0.0270757, -0.0232523,
// 0.0390650,  0.9729502, -0.0092579,
//-0.0024047,  0.0026446,  0.9180873
 0.9531874, -0.0265906, 0.0238731,
-0.0382467,  1.0288406, 0.0094060,
 0.0026068, -0.0030332, 1.0892565
};

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
	if (argc < 3) {
		printf("Usage: convert input output\n");
		exit(-1);
	}

	Image src;
	src.load(argv[1]);
	
	Image dst(src.width, src.height, src.channels);
	// ignore alpha channel
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
		float xyzD65[3] = {0};
		float xyzE[3] = {0};

		transform(to_XYZ, csrc, xyzD65);
		transform(B, xyzD65, xyzE);
		transform(from_XYZ, xyzE, cdst);

		dst.data[offset + 0] = cdst[0];
		dst.data[offset + 1] = cdst[1];
		dst.data[offset + 2] = cdst[2];
	}

	dst.save(argv[2]);

	return 0;
}
