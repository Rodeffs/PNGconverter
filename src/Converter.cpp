#include "../inc/Converter.hpp"
#include <cstdio>

Converter::Converter(char* inputFilePath, char* outputFilePath, unsigned int selectDepth, bool isGray) {

	inputFile = fopen(inputFilePath, "rb");

	outputFile = fopen(outputFilePath, "wb");

	depth = selectDepth;

	grayscale = isGray;
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

void Converter::setPixelDepth(unsigned int newDepth) {
	depth = newDepth;
}

void Converter::setGrayscale(bool answer) {
	grayscale = answer;
}

unsigned int Converter::getPixelDepth() {
	return depth;
}

bool Converter::getGrayscale() {
	return grayscale;
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

bool Converter::checkParameters() {
	
	if (depth == 8 || depth == 16 || depth == 24 || depth == 48) {
		if (grayscale && (depth > 16)) {
			cout << "Error, grayscale can only be either 8 or 16 pixels in depth. Use -h or --help to get help" << endl;
			return false;
		}
		return true;
	}

	cout << "Error, invalid pixel depth. Use -h or --help to get help" << endl;
	return false;
}

/* So the way I plan to convert the file to PNG is this:
 * 1. Read the input file byte by byte
 * 2. Depending on the chosen settings, either encode each byte as one pixel, or group several bytes together to encode one pixel
 *
 * The settings are:
 * 1. The resulting PNG resolution is (at least by default) a X by X square, where X is ceil(sqrt(initial file size / pixel size))
 * 2. All empty pixels (encoded with zeros) are just fully transparent
 * 3. To yield better results, I think alpha channel needs to be disabled, plus easier to encode and less space
 * 4. User may choose one of three types of PNG: Palette-Based, Grayscale and RGB
 * 5. Since I plan to read the data byte by byte, only allowed pixel depths are: 8 bits for Palette-Based, 8 and 16 bits for Grayscale; 24 and 48 bits for RGB
 * 5.1. The way it's going to be chosen is this: switch -p will select the pixel depth and switch -g (if present) will make the image grayscale. If switch -g is not used, then user may choose between 8, 24 and 48 bits, where 8 bit will default to Palette-Based. Otherwise, user may choose between 8 and 16 bits, all other values will result in an error
 * 5.2. The default is 8 bit Palette-Based image
 *
 * All of this data is directly encoded into PNG, so no further configuration is needed
 */

void Converter::convert() {

	if (!checkFiles() || !checkParameters())
		return;

	
}

/* The deconversion is the same, but in reverse:
 * 1. Get all the relevant information about the image (type, depth, resolution)
 * 2. Decode the image by reading pixels in rows and converting the color values back to original bytes
 */

void Converter::deconvert() {

	if (!checkFiles())
		return;

	if (!pngUtils::checkIfPng(inputFile)) {
		cout << "Error, the input file is not a PNG" << endl;
		return;
	}

}

