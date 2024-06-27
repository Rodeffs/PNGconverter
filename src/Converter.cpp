#include "../inc/Converter.hpp"

Converter::Converter(char* inputFilePath, char* outputFilePath) {

	inputFile = fopen(inputFilePath, "rb");

	outputFile = fopen(outputFilePath, "wb");

}

Converter::~Converter() {

	fclose(inputFile);

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
