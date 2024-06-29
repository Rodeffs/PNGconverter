#pragma once

#include <cstdio>
#include <cstdint>
#include <png.h>  // the C libpng library used to read/write PNGs

class ImagePNG {

private:
	uintmax_t imageSize = 0;

public:
	ImagePNG() = default;
	
	~ImagePNG() = default;

	int checkIfPNG(FILE* file);

	void write(unsigned char* data, FILE* outputFile, uintmax_t width, uintmax_t height, uintmax_t dataSize); // writes pixel data to PNG

	unsigned char* read(FILE* inputFile); // reads pixel data fron PNG
	
	uintmax_t getImageSize();
};	

