#pragma once

#include "libraries.hpp"

#include "pngUtils.hpp"
#include <cstdint>

class Converter {

private:
	FILE* inputFile;

	FILE* outputFile;

	uintmax_t inputFileSize;

	unsigned int depth;

	bool grayscale;

	bool checkFiles();

	bool checkParameters();

public:
	Converter(char* inputFilePath, char* outputFilePath, unsigned int selectDepth = 8, bool isGray = false);
	
	~Converter();

	void setInputFile(char* newInputFilePath);

	void setOutputFile(char* newOutputFilePath);

	FILE* getInputFile();

	FILE* getOutputFile();

	void setPixelDepth(unsigned int newDepth);

	void setGrayscale(bool answer);

	unsigned int getPixelDepth();

	bool getGrayscale();

	void convert();

	void deconvert();

};
