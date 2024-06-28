#pragma once

#include "pngUtils.hpp"
#include <exception>
#include <cstdint>
#include <cstdio>
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
	char* inputFilePath;

	char* outputFilePath;

	uintmax_t inputFileSize;

	unsigned char* readBytes(int extraBytes);  // reads the original file byte by byte and adds extra info needed for decoding

	void calculateInputFileSize();

public:
	Converter(char* inFilePath, char* outFilePath);
	
	~Converter() = default;

	void setInputFilePath(char* newInputFilePath);

	void setOutputFilePath(char* newOutputFilePath);

	char* getInputFilePath();

	char* getOutputFilePath();

	uintmax_t getInputFileSize();

	void encode();

	void decode();
};
