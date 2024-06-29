#pragma once

#include <cstdint> // there technically isn't much difference between writing uintmax_t and unsigned int, 
		   // but in theory it means that larger files can be processed, plus it's shorter to type
#include <cstdio>

class Converter {

private:
	FILE* inputFile = nullptr;
	
	FILE* outputFile = nullptr;

	uintmax_t outputImageWidth = 0;

	uintmax_t outputImageHeight = 0;

	uintmax_t inputFileSize = 0;

	unsigned char* intToBytes(uintmax_t value);

	uintmax_t bytesToInt(unsigned char* byteData);

	unsigned char* readBytes(uintmax_t extraBytes);  // reads the original file byte by byte and adds extra bytes for decoding
	
	bool findBestResolution();

public:
	Converter() = default;

	Converter(char* inputFilePath, char* outputFilePath);

	~Converter();

	void setInputOutputFiles(char* inputFilePath, char* outputFilePath);

	void setInputFile(char* inputFilePath);

	void setOutputFile(char* outputFilePath);
	
	void setOutputImageResolution(uintmax_t width, uintmax_t height);

	void setOutputImageWidth(uintmax_t width);

	void setOutputImageHeight(uintmax_t height);

	uintmax_t getOutputImageWidth();

	uintmax_t getOutputImageHeight();

	void encode();

	void decode();
};
