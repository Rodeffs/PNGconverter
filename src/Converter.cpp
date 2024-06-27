#include "../inc/Converter.hpp"

Converter::Converter(char* inputFilePath, char* outputFilePath, bool isGray) {

	inputFile = fopen(inputFilePath, "rb");

	outputFile = fopen(outputFilePath, "wb");

	grayscale = isGray;
	
	try {
		inputFileSize = fs::file_size(inputFilePath);  // since we need FileSize later
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		return;
	}
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

void Converter::setGrayscale(bool answer) {
	grayscale = answer;
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

/* So the way I plan to convert the file to PNG is this:
 * 1. Each byte from the original file is encoded to a pixel (8-bit pixel depth, aka 1 byte from original file = 1 byte encoded pixel)
 * 2. The resulting PNG resolution is an X by X pixels, where X is the square root of the original file size rounded up
 * 3. All unused pixels are just transparent
 * 4. User may choose for PNG to either have colors (Palette-Based) or to be black & white with shades of gray (Grayscale)
 *
 * A file size can NOT be a non-integer value, therefore it can always be encoded and decoded byte by byte unambiguously with 8-bit pixel depth.
 * For this reason, if I were to choose 24 bit pixel depth (3 bytes), then if file size is not divisible by 3 I'd have to fill unused data with zeros.
 * It's not a problem for encoding, but it makes decoding ambiguous because I can't know for sure if those zeros were there originally or were added by the program.
 * Besides, who cares, 8-bit color is good enough, plus you can make it black & white.
 *
 */

void Converter::encode() {

	if (!checkFiles())
		return;
	
	auto resolution = ceil(sqrt(inputFileSize));
}

/* The decoding is the same, but in reverse:
 * 1. Get all the relevant information about the image (type, depth, resolution)
 * 2. Decode the image by reading pixels in rows and converting the color values back to original bytes
 */

void Converter::decode() {

	if (!checkFiles())
		return;

	if (!pngUtils::checkIfPng(inputFile)) {
		cout << "Error, the input file is not a PNG" << endl;
		return;
	}

}

