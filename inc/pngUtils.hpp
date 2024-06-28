#pragma once

#include <cstdint>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <png.h>  // the C libpng library used to read/write PNGs

using std::cout;
using std::endl;
using std::strcmp;

enum class Access {READ, WRITE};

class ImagePNG {

private:
	png_structp png_ptr;

	png_infop info_ptr;

	FILE* currentFile;

	uintmax_t height;

	uintmax_t width;

	Access accessType;

	bool checkCurrentFile();

	bool isCreated = false;

public:
	ImagePNG(char* filePath, const char* mode); // opens or creates PNG from filePath (mode is "wb" for writing and "rb" for reading)
	
	~ImagePNG();

	int checkIfPng();

	void create(uintmax_t width, uintmax_t height);

	void write(unsigned char* data); // writes pixel data to PNG

	unsigned char* read(); // reads pixel data fron PNG, ignoring transparent pixels
};	
	
