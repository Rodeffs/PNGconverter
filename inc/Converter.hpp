#pragma once

#include "libraries.hpp"

#include "pngUtils.hpp"

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
