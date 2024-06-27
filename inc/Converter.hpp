#pragma once

#include "libraries.hpp"

#include "pngUtils.hpp"

class Converter {

private:
	FILE* inputFile;

	FILE* outputFile;

public:
	Converter(char* inputFilePath, char* outputFilePath);
	
	~Converter();

	void setInputFile(char* newInputFilePath);

	void setOutputFile(char* newOutputFilePath);

	FILE* getInputFile();

	FILE* getOutputFile();

	bool checkFiles();

	void convert();

	void deconvert();

};
