#include <image.h>

#include <cassert>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>

static void 
hdr_to_ldr(
		const float* src, 
		std::size_t w,
		std::size_t h, 
		std::size_t c,
		unsigned char* dst)
{
	for (std::size_t j = 0; j < h; ++j)
	for (std::size_t i = 0; i < w; ++i)
	{
		std::size_t offset = c * (w * j + i);
		for (std::size_t k = 0; k < c; ++k) {
			dst[offset + k] = static_cast<unsigned char>(
				std::min(255, std::max(0, int(src[offset+k] * 255))));
		}
	}
}

static void 
ldr_to_hdr(
		const unsigned char* src, 
		std::size_t w,
		std::size_t h, 
		std::size_t c,
		float* dst)
{
	for (std::size_t j = 0; j < h; ++j)
	for (std::size_t i = 0; i < w; ++i)
	{
		std::size_t offset = c * (w * j + i);
		for (std::size_t k = 0; k < c; ++k) {
			dst[offset + k] = static_cast<float>(src[offset+k])/255.f;
		}
	}
}

static int 
load_exr(const char* filename, Image* img)
{
	float* src;
	const char* err;
	int w, h;
	int ret = LoadEXR(&src, &w, &h, filename, &err);
	if (ret != TINYEXR_SUCCESS) {
		fprintf(stderr, "error loading exr %s: %s\n", filename, err);
		return 1;
	}
	img->width = w;
	img->height = h;
	img->channels = 4;
	img->data.resize(img->size());
	memcpy(img->data.data(), src, img->byte_size());
	return 0;
}

static int 
save_exr(const char* filename, const Image* img)
{
	assert(img->channels == 4);
	return SaveEXR(img->data.data(), img->width, img->height, img->channels, 
				   0, filename);
}

inline bool 
little_endian() 
{
	const int i = 1; 
	return (*reinterpret_cast<char const*>(&i) == 1); 
}

inline void 
readCommentsAndEmptyLines(std::ifstream& file)
{ 
	std::string line;
	while (true) 
	{
		std::streampos pos = file.tellg();
		getline(file, line); 
		if (line.length() > 0 && line.at(0) == '#') { 
		}
		else { 
			file.seekg(pos);
			break;
		}
	}
}

static int 
load_pfm(const char* filename, Image* img)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file || !file.is_open()) {
		fprintf(stderr, "error reading pfm %s\n", filename);
		return 1;
	}
	std::string line;     // read file type 
	getline(file, line);
	readCommentsAndEmptyLines(file);
	
	// read width, height
	int width, height;     
	getline(file, line);
	std::sscanf(line.c_str(), "%i %i", &width, &height);
	assert(width > 0 && height > 0);

	// read endianess
	int endian;     
	getline(file, line);
	std::sscanf(line.c_str(), "%i", &endian);
	bool little_endian_read = endian < 0 ? true : false;
	if(little_endian_read != little_endian()) 
	{ 
		fprintf(stderr, "file has different endianness.\n");
		return 1;
	}     

	img->width = width;
	img->height = height;
	img->channels = 3; // only RGB is supported XXX: add R support
	img->data.resize(img->size());
	file.read(reinterpret_cast<char*>(img->data.data()),
			  static_cast<std::streamsize>(img->byte_size()));

	if (!file) 
	{ 		
		fprintf(stderr, "error reading pfm %s\n", filename);
		return 1;     
	} 	

	file.close();
	return 0;
}

static int 
save_pfm(const char* filename, const Image* img)
{
	std::ofstream of(filename, std::ios::out | std::ios::binary);
	if (!of) 
	{
		fprintf(stderr, "error writing pfm %s\n", filename);
		return 1;
	}
	
	// write header.
	if (img->channels == 3 || 4) {
		of << "PF\n"; // color image.     
	}  
	else if (img->channels == 1) {
		of << "Pf\n"; // grayscale image.     
	}  
	else 
	{
		fprintf(stderr, "cannot write pfm file with %ld channels\n",
				img->channels);
		return 1;
	} 
	of << img->width << " " << img->height << "\n"
	   << (little_endian() ? "-1" : "1") << "\n" 
	   << std::flush;

	char const* data = reinterpret_cast<char const*>(img->data.data());
	size_t size = img->byte_size();

	std::vector<float> rgb;
	if (img->channels == 4) {
		// need to rip out alpha channel
		rgb.resize(img->width * img->height * 3);
		for(size_t j = 0; j < img->height; ++j)
		for(size_t i = 0; i < img->width;  ++i)
		{
			size_t offset = img->width * j + i;
			rgb[3*offset+0] = img->data[4*offset+0];
			rgb[3*offset+1] = img->data[4*offset+1];
			rgb[3*offset+2] = img->data[4*offset+2];
		}
		data = reinterpret_cast<char const*>(rgb.data());
		size = img->width * img->height * 3 * sizeof(float);
	}

	of.write(data, static_cast<std::streamsize>(size));

	if (!of) { 
		fprintf(stderr, "error writing pfm %s\n", filename);
	}
	return 0;
}

static int 
load_stbi(const char* filename, Image* img)
{
	int width, height, n;
	unsigned char *rgb = stbi_load(filename, &width, &height, &n, 0);

	img->width = width;
	img->height = height;
	img->channels = n;
	img->data.resize(img->size());

	ldr_to_hdr(rgb, width, height, n, img->data.data());
	return 0;
}

static int 
save_png(const char* filename, const Image* img)
{
	std::vector<unsigned char> ldr(img->size());
	hdr_to_ldr(img->data.data(), img->width, img->height, img->channels, 
			   ldr.data());
	return !stbi_write_png(filename, img->width, img->height, img->channels, 
		                   ldr.data(), 0);
}

static int 
save_jpg(const char* filename, const Image* img)
{
	std::vector<unsigned char> ldr(img->size());
	hdr_to_ldr(img->data.data(), img->width, img->height, img->channels, 
			   ldr.data());
	return !stbi_write_jpg(filename, img->width, img->height, img->channels, 
		                   ldr.data(), 100);
}

static std::string
extension(std::string const& filepath)
{
	// first get filename of path to prevent issues with files without
	// extensions but dots in the directory names
	std::string filename = filepath;
	std::string extension = "";
	std::size_t i = filepath.rfind('/', filepath.length());
	if (i != std::string::npos) {
		filename =  filepath.substr(i+1, filepath.length() - i);
	}
	i = filename.rfind('.', filename.length());
	if (i != std::string::npos) {
		extension =  filename.substr(i+1, filename.length() - i);
	}
	return extension;
}

void Image::
load(const char* filename)
{
	std::string ext = extension(filename);
	int ret = 0;
	if (ext == "exr") {
		ret = load_exr(filename, this);
	}
	else if (ext == "pfm") {
		ret = load_pfm(filename, this);
	}
	else if (ext == "png" || ext == "jpg" || ext == "bmp" || 
			 ext == "tga" || ext == "hdr") 
	{
		ret = load_stbi(filename, this);
	}
	else {
		fprintf(stderr, "unsupported extension for load %s\n", ext.c_str());
	}
	if (ret) {
		fprintf(stderr, "error loading file %s\n", filename);
	}
}

void Image::
save(const char* filename)
{
	std::string ext = extension(filename);
	int ret = 0;
	if (ext == "exr") {
		ret = save_exr(filename, this);
	}
	else if (ext == "pfm") {
		ret = save_pfm(filename, this);
	}
	else if (ext == "png") {
		ret = save_png(filename, this);
	}
	else if (ext == "jpg") {
		ret = save_jpg(filename, this);
	}
	else {
		fprintf(stderr, "unsupported extension for save %s\n", ext.c_str());
	}
	if (ret) {
		fprintf(stderr, "error writing file %s\n", filename);
	}
}

