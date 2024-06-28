#pragma once

#include <cstdio>
#include <cstdint>
#include <png.h>  // the C libpng library used to read/write PNGs

class ImagePNG {

private:
	png_structp png_ptr;

	png_infop info_ptr;

public:
	ImagePNG();
	
	~ImagePNG();

	int checkIfPng(FILE* file);

	void write(unsigned char* data, FILE* outputFile, uintmax_t width, uintmax_t height); // writes pixel data to PNG

	unsigned char* read(FILE* inputFile); // reads pixel data fron PNG, ignoring transparent pixels
};	
	
