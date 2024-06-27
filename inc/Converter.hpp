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
	FILE* inputFile;

	FILE* outputFile;

	uintmax_t inputFileSize;

	bool checkFiles();

public:
	Converter(char* inputFilePath, char* outputFilePath);
	
	~Converter();

	void setInputFile(char* newInputFilePath);

	void setOutputFile(char* newOutputFilePath);

	FILE* getInputFile();

	FILE* getOutputFile();

	void encode();

	void decode();
};
