#pragma once

#include <cstdint>
#include <cstdio>

struct Resolution {

	uintmax_t width;

	uintmax_t height;
};

class Converter {

private:
	unsigned char* intToBytes(uintmax_t value);

	uintmax_t bytesToInt(unsigned char* byteData);

	unsigned char* readBytes(FILE* inputFile, uintmax_t inputFileSize, uintmax_t extraBytes);  // reads the original file byte by byte and adds extra bytes for decoding
	
	Resolution* bestResolution(uintmax_t fileSize);

	bool checkFiles(FILE* inputFile, FILE* outputFile);

public:
	Converter() = default;
	
	~Converter() = default;

	void encode(char* inputFilePath, char* outputFilePath);

	void decode(char* inputFilePath, char* outputFilePath);
};
