#pragma once

#include "pngUtils.hpp"
#include <exception>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <filesystem>

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

	bool grayscale;

	bool checkFiles();

public:
	Converter(char* inputFilePath, char* outputFilePath, bool isGray = false);
	
	~Converter();

	void setInputFile(char* newInputFilePath);

	void setOutputFile(char* newOutputFilePath);

	FILE* getInputFile();

	FILE* getOutputFile();

	void setGrayscale(bool answer);

	bool getGrayscale();

	void encode();

	void decode();
};
