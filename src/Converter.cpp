#include "../inc/Converter.hpp"

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

void Converter::execute(Action action) { // I wrote execute so that I won't have to checkFiles() twice for conversion/deconversion
	
	if (!checkFiles())
		return;

	if (action == Action::CONVERT)
		convert();

	else if (action == Action::DECONVERT)
		deconvert();
}

void Converter::deconvert() {

	if (!pngUtils::checkIfPng(inputFile)) {
		cout << "Error, the input file is not a PNG" << endl;
		return;
	}

	cout << "The input file is a PNG" << endl;
}

void Converter::convert() {
	return;
}
