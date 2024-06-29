#pragma once

#include <cstdint>
#include <cstdio>

class Converter {

private:
	FILE* inputFile = nullptr;
	
	FILE* outputFile = nullptr;

	unsigned int outputImageWidth = 0;

	unsigned int outputImageHeight = 0;

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
	
	void setOutputImageResolution(unsigned int width, unsigned int height);

	void setOutputImageWidth(unsigned int width);

	void setOutputImageHeight(unsigned int height);

	unsigned int getOutputImageWidth();

	unsigned int getOutputImageHeight();

	void encode();

	void decode();
};
