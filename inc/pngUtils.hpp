#pragma once

#include <cstdio>
#include <cstdint>
#include <png.h>  // the C libpng library used to read/write PNGs

class ImagePNG {

private:
	png_structp png_ptr;

	png_infop info_ptr;

	uintmax_t readDataSize = 0; // a bit crappy way to get information about pixel data size

public:
	ImagePNG() = default;
	
	~ImagePNG() = default;

	int checkIfPNG(FILE* file);

	void write(unsigned char* data, FILE* outputFile, uintmax_t width, uintmax_t height, uintmax_t dataSize); // writes pixel data to PNG

	unsigned char* read(FILE* inputFile); // reads pixel data fron PNG
	
	uintmax_t getReadDataSize();
};	

