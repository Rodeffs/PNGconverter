#pragma once

#include "libraries.hpp"

enum class Action {CONVERT, DECONVERT};

class Converter {

private:
	FILE* inputFile;

	FILE* outputFile;

	void convert();

	void deconvert();

public:
	Converter(char* inputFilePath, char* outputFilePath);
	
	~Converter();

	bool checkFiles();

	void execute(Action action);
};
