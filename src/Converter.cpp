#include "../inc/Converter.hpp"
#include "../inc/pngUtils.hpp"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <cmath>
#include <iostream>

using std::fopen;
using std::fclose;
using std::cout;
using std::endl;
using std::sqrt;
using std::perror;
using std::fgetc;
using std::fputc;
using std::ferror;

namespace fs = std::filesystem;

Converter::Converter(char* inputFilePath, char* outputFilePath) {
	
	inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error in converter, unable to open input file");
		inputFile = nullptr;
		return;
	}
	
	inputFileSize = fs::file_size(inputFilePath);

	outputFile = fopen(outputFilePath, "wb");
	
	if (!outputFile) {
		perror("Error in converter, unable to open output file");
		outputFile = nullptr;
		return;
	}
}

Converter::~Converter() {
	
	if (inputFile)
		fclose(inputFile);

	if (outputFile)
		fclose(outputFile);
}

void Converter::setInputOutputFiles(char* inputFilePath, char* outputFilePath) {
	
	// Never forget to close old files before changing!

	if (inputFile)
		fclose(inputFile);

	if (outputFile)
		fclose(outputFile);

	inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error in converter, unable to open input file");
		inputFile = nullptr;
		return;
	}
	
	inputFileSize = fs::file_size(inputFilePath);

	outputFile = fopen(outputFilePath, "wb");
	
	if (!outputFile) {
		perror("Error in converter, unable to open output file");
		outputFile = nullptr;
		return;
	}
}

void Converter::setInputFile(char* inputFilePath) {
	
	// Never forget to close old files before changing!

	if (inputFile)
		fclose(inputFile);

	inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error in converter, unable to open input file");
		inputFile = nullptr;
		return;
	}
	
	inputFileSize = fs::file_size(inputFilePath);
}

void Converter::setOutputFile(char* outputFilePath) {
	
	// Never forget to close old files before changing!

	if (outputFile)
		fclose(outputFile);

	outputFile = fopen(outputFilePath, "wb");
	
	if (!outputFile) {
		perror("Error in converter, unable to open output file");
		outputFile = nullptr;
		return;
	}
}

void Converter::setOutputImageResolution(uintmax_t width, uintmax_t height) {

	outputImageWidth = width, outputImageHeight = height;
}

void Converter::setOutputImageWidth(uintmax_t width) {

	outputImageWidth = width;
}

void Converter::setOutputImageHeight(uintmax_t height) {

	outputImageHeight = height;
}

uintmax_t Converter::getOutputImageWidth() {

	return outputImageWidth;
}

uintmax_t Converter::getOutputImageHeight() {

	return outputImageHeight;
}

// Actually useful code begins here

unsigned char* Converter::intToBytes(uintmax_t value) {
	
	// Convert the uintmax_t amount of extra bytes to bytes to be encoded into PNG

	unsigned char* convert = new unsigned char[8];

	for (int i = 7; i >= 0; i--) {
		convert[i] = value % 256;
		value >>= 8;
	}

	return convert;
}

uintmax_t Converter::bytesToInt(unsigned char* byteData) {

	// Decode the amount of extra bytes

	uintmax_t decoded = 0;

	for (int i = 0; i < 8; i++) {
		decoded <<= 8;
		decoded += byteData[i];
	}

	return decoded;
}


unsigned char* Converter::getByteData() {
	
	// The size of pixel data

	uintmax_t totalSize = outputImageHeight * outputImageWidth * 3;

	unsigned char* byteData = new unsigned char[totalSize];
	
	// The amount of extra bytes added at the end

	auto extraBytesAmount = intToBytes(totalSize - 9 - inputFileSize);
	
	// The whole encoding in one cycle

	for (uintmax_t i = 0; i < totalSize; i++) {
		
		// Encoding first 9 bytes
		
		/*
		 * Okay, so technically only the first 8 bytes would be enough, however that would mean that
		 * the visualized data is not really representative of the original, since we shifted
		 * it to fit the first 3 pixels. For the sake of better visualization I decided to make
		 * first 3 pixels store ONLY the information about the extra bytes at the end, instead of
		 * a third pixel storing the first 2 bytes of that info and the last byte being the encoded
		 * data itself
		 */
		
		if (i < 8)
			byteData[i] = extraBytesAmount[i];

		else if (i == 8)
			byteData[i] = 0; // so yeah, it means that the last byte is always 0, because 3 pixels = 9 bytes
					 // and unsigned int = 8 bytes, but sacrificing only 1 byte is not that bad 

		// Encoding the input file itself

		else if (i < (9 + inputFileSize)) {

			byteData[i] = fgetc(inputFile);
			
			// Error checking
			
			if (ferror(inputFile)) {
				cout << "Error while reading data from the input file" << endl;
				delete[] extraBytesAmount;
				delete[] byteData;
				return nullptr;
			}
		}

		// Adding extra bytes if there are some

		else
			byteData[i] = 0;
	}

	delete[] extraBytesAmount;
	
	return byteData;
} 

bool Converter::findBestResolution() {

/* The idea is to store image using as less pixels as possible, while also making it resemble a square as much as possible.
 * We need to know how many pixels the original file takes and then add 3 reserved bytes to the beginning
 * Instead of relying on std::floor I made it so the program adds the extra 1-2 bytes in case the resulting file size is not divisible by 3 
 * Next, we calculate the square root of the result, round it down to an integer and multiply it by itself
 * If the result is equal or greater than the total pixel count, we use it
 * If it's less we first increment width by 1 and if it's still not enough, then also height by 1
 */

	int toBeDivisibleBy3 = (3 - (inputFileSize % 3)) % 3;

	uintmax_t totalPixels = 3 + (inputFileSize + toBeDivisibleBy3) / 3;
	
	outputImageHeight = sqrt(totalPixels);

	outputImageWidth = outputImageHeight;

	while (outputImageHeight * outputImageWidth < totalPixels) {
		if (outputImageWidth == outputImageHeight)
			outputImageWidth++;
		else
			outputImageHeight++;
	}
	
	if (outputImageHeight > 1000000 || outputImageWidth > 1000000) { // libpng default max height and width, otherwise might be buffer overflow 
		cout << "Error, the input file is too large" << endl;
		return false;
	}
	
	return true;
}

/* So the way I plan to convert the file to PNG is this:
 * 1. Add 8 bytes to the beginning which is an 64-bit unsigned integer
 * 2. Either use user defined resolution or if it's to small, calculate a better one
 * 3. The 64 bit integer stores the amount of extra bytes that were added at the end to make file fit the resolution
 * 4. The byte data is then converted into pixel data and added to png
 */

void Converter::encode() {

	if (!inputFile || !outputFile) {
		cout << "Error, input/output files weren't provided" << endl;
		return;
	}
	
	if ( (outputImageHeight * outputImageWidth * 3 < (9 + inputFileSize)) || (outputImageHeight > 1000000) || (outputImageWidth > 1000000) )
		if (!findBestResolution())
			return;
	
	auto byteData = getByteData();

	if (!byteData)
		return;

	ImagePNG outputPNG;

	outputPNG.write(byteData, outputFile, outputImageWidth, outputImageHeight);

	delete[] byteData;
}

/* The decoding is the same, but in reverse:
 * 1. Read the first 8 bytes to get the value of excessive bytes at the end, say X
 * 1. Continue reading the PNG pixel by pixel, row by row
 * 2. Every pixel color value is split into three bytes corresponding to the color
 * 3. Each byte is appended to output file
 * 4. The last X bytes are ignored.
 */

void Converter::decode() {

	if (!inputFile || !outputFile) {
		cout << "Error, input/output files weren't provided" << endl;
		return;
	}

	ImagePNG inputPNG;

	auto byteData = inputPNG.read(inputFile);

	if (!byteData)
		return;

	uintmax_t extraBytesAmount = bytesToInt(byteData), byteDataSize = inputPNG.getImageSize();
	
	for (uintmax_t i = 9; i < (byteDataSize - extraBytesAmount); i++)
		fputc(byteData[i], outputFile);

	delete[] byteData;

	if (ferror(outputFile)) {
		cout << "Error while writing data to the output file" << endl;
		fclose(outputFile); // immediately close the output file before the destructor does that so no further corruption
	}
}

