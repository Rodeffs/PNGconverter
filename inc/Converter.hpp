#pragma once

#include "pngUtils.hpp"
#include <exception>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <filesystem>
#include <png.h>  // the libpng library used to read/write PNGs

using std::perror;
using std::fopen;
using std::fclose;
using std::cout;
using std::endl;
using std::sqrt;
using std::ceil;

class Converter {

private:
	char* inputFilePath;

	char* outputFilePath;

	uintmax_t inputFileSize;

	unsigned char* readBytes(int extraBytes);  // to read the original file byte by byte and store it on heap


public:
	Converter(char* inFilePath, char* outFilePath);
	
	~Converter() = default;

	void encode();

	void decode();
};
