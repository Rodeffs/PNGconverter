#include "../inc/Converter.hpp"
#include <cstdio>

Converter::Converter(char* inputFilePath, char* outputFilePath) {

	inputFile = fopen(inputFilePath, "rb");

	outputFile = fopen(outputFilePath, "wb");

}

Converter::~Converter() {
	
	if (inputFile)
		fclose(inputFile);
	
	if (outputFile)
		fclose(outputFile);
}

void Converter::setInputFile(char* newInputFilePath) {

	if (inputFile)
		fclose(inputFile);

	inputFile = fopen(newInputFilePath, "rb");
}

void Converter::setOutputFile(char* newOutputFilePath) {

	if (outputFile)
		fclose(outputFile);

	outputFile = fopen(newOutputFilePath, "wb");
}

FILE* Converter::getInputFile() {
	return inputFile;
}

FILE* Converter::getOutputFile() {
	return outputFile;
}

bool Converter::checkFiles() {

	if (!inputFile) {
		perror("Error, unable to open input file");
		return false;
	}

	if (!outputFile) {
		perror("Error, unable to open output file");
		return false;
	}

	return true;
}

void Converter::deconvert() {

	if (!checkFiles())
		return;

	if (!pngUtils::checkIfPng(inputFile)) {
		cout << "Error, the input file is not a PNG" << endl;
		return;
	}

	cout << "The input file is a PNG" << endl;
}

void Converter::convert() {

	if (!checkFiles())
		return;
}
