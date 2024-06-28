#pragma once

#include <cstdint>
#include <cstdio>
#include <cmath>
#include <iostream>

using std::perror;
using std::fopen;
using std::fclose;
using std::cout;
using std::endl;
using std::sqrt;
using std::ceil;
using std::floor;

struct Resolution {

	uintmax_t width;

	uintmax_t height;
};

class Converter {

private:
	char* inputFilePath;

	char* outputFilePath;

	uintmax_t inputFileSize;

	unsigned char* intToBytes(uintmax_t value);

	uintmax_t bytesToInt(unsigned char* byteData);

	unsigned char* readBytes(uintmax_t extraBytes);  // reads the original file byte by byte and adds extra bytes for decoding
	
	Resolution* bestResolution();

	void calculateInputFileSize();

public:
	Converter(char* inFilePath, char* outFilePath);
	
	~Converter() = default;

	void setInputFilePath(char* newInputFilePath);

	void setOutputFilePath(char* newOutputFilePath);

	char* getInputFilePath();

	char* getOutputFilePath();

	void encode();

	void decode();
};
