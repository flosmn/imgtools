#include <cstdio>
#include <vector>
#include <cstring>
#include <cmath>

#include <image.h>

static float rye[5][3] = {
	{ 0.0f, 0.0f, 0.5f },
	{ 0.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 0.5f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
};

static inline void lerp(
		float w, 
		const float c0[3], 
		const float c1[3], 
		float c[3])
{
	c[0] = w * c1[0] + (1.f - w) * c0[0];
	c[1] = w * c1[1] + (1.f - w) * c0[1];
	c[2] = w * c1[2] + (1.f - w) * c0[2];
}

static inline void color_palette(float w, float color[3])
{
	if (w < 0.25f) 
	{
		w /= 0.25;
		lerp(w, rye[0], rye[1], color);
	}
	else if (w < 0.5f)
	{
		w -= 0.25;
		w /= 0.25;
		lerp(w, rye[1], rye[2], color);
	}
	else if (w < 0.75)
	{
		w -= 0.5;
		w /= 0.25;
		lerp(w, rye[2], rye[3], color);
	}
	else 
	{
		w -= 0.75;
		w /= 0.25;
		lerp(w, rye[3], rye[4], color);
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: convert output\n");
		exit(-1);
	}
	
	Image dst(512, 32, 3);
	
	// ignore alpha channel
	for (std::size_t y = 0; y < dst.height; ++y)
	for (std::size_t x = 0; x < dst.width;  ++x)
	{
		std::size_t offset = dst.channels * (dst.width * y + x);

		float alpha = ((float)x+0.5f)/dst.width;

		float color[3];
		color_palette(alpha, color);

		for (int c = 0; c < 3; ++c) {
			dst.data[offset + c] = color[c];
		}
	}

	dst.save(argv[1]);

	return 0;
}
