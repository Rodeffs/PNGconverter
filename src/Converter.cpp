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

void Converter::setOutputImageResolution(unsigned int width, unsigned int height) {

	outputImageWidth = width, outputImageHeight = height;
}

void Converter::setOutputImageWidth(unsigned int width) {

	outputImageWidth = width;
}

void Converter::setOutputImageHeight(unsigned int height) {

	outputImageHeight = height;
}

unsigned int Converter::getOutputImageWidth() {

	return outputImageWidth;
}

unsigned int Converter::getOutputImageHeight() {

	return outputImageHeight;
}

// Actually useful code begins here

unsigned char* Converter::intToBytes(uintmax_t value) {
	
	// Convert the unsigned int amount of extra bytes to bytes to be encoded into PNG

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

	for (int i = 1; i < 9; i++) {
		decoded <<= 8;
		decoded += byteData[i];
	}

	return decoded;
}


unsigned char* Converter::readBytes(uintmax_t extraBytes) {

	unsigned char* data = new unsigned char[9 + inputFileSize + extraBytes];

	// Encoding first three pixels

	data[0] = 0;

	uintmax_t i = 0;

	auto extraValues = intToBytes(extraBytes);

	for (i; i < 9; i++)
		data[i] = extraValues[i-1];

	delete[] extraValues;
	
	// Converting the initial data into byte data
	
	for (i; i < (9 + inputFileSize); i++)
		data[i] = fgetc(inputFile); 

	// Adding extra bytes if there are any

	for (uintmax_t j = 0; j < extraBytes; j++) 
		data[i + j] = 0;
	
	return data;
} 

bool Converter::findBestResolution() {

/* The idea is to store image using as less pixels as possible, while also making it resemble a square as much as possible.
 * To do that first we need to know how many pixels the original file takes, which we can calculate by dividing the original
 * byte size by 3 (every pixel is 3 bytes), rounding it up and adding the reserved first three pixels.
 * Next, we calculate the square root of the result, round it down and use the number as the current resolution.
 * Then we find the amount of leftover pixels if we were to use this resolution.
 * If there are none, then we keep it
 * If there is some, then we increase the width by 1 and substract this value from the leftover pixels
 * If the resulting number is positive, it means there are still some leftover pixels left, which we can include by
 * increasing the height as well
 */
	// In case the initial file size is not divisible by 3, add this amount of extra bytes, so it can be converted to pixels

	int toBeDivisibleBy3 = (3 - inputFileSize % 3) % 3;

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
 * 1. Calculate resolution
 * 2. The first three pixels (72 bits of data) are reserved and store information about the excess bytes, which is calculated
 * by substracting original file size from X
 * 3. Each byte from the original file is converted into a RGB value (from 0 to 255) and stored to an array.
 * 4. Then extra bytes are appended to the array as random integers from 0 to 255.
 * 5. Every three bytes from the array are read and converted into an RGB value (the amount of bytes will always be divisible by 3) for a single pixel
 * 6. This pixel is added to the PNG and the process continues row by row until the PNG is fully encoded.
 *
 * Since unsigned int size is 64 bits (8 bytes) for most platforms and a pixel is encoded with 24 bits (3 bytes) of data, I had to reserved
 * the first 3 pixels (72 bits = 9 bytes) to store that info, which means that the first byte of the first reserved pixel will always be 0 (it will never have red).
 * As I don't know how to encode transparency with this library (and TBH I don't really want to know) and using alpha-channels would double
 * the PNG size, I think this solution is okay.
 * It would only fail in case the amount of excess data is 2^64 bytes, which would require the initial file size prior to encoding to be
 * at least 2^128 - 2^64 bytes, roughly 2^68 EiB of data, waaaayyy more than any file system can store 
 */

void Converter::encode() {

	if (!inputFile || !outputFile) {
		cout << "Error, input/output files weren't provided" << endl;
		return;
	}
	
	if ( (outputImageHeight * outputImageWidth * 3 < (9 + inputFileSize)) || (outputImageHeight > 1000000) || (outputImageWidth > 1000000) )
		if (!findBestResolution())
			return;
	
	uintmax_t extraBytes = outputImageHeight * outputImageWidth * 3 - 9 - inputFileSize;

	auto byteData = readBytes(extraBytes);

	ImagePNG outputPNG;

	outputPNG.write(byteData, outputFile, outputImageWidth, outputImageHeight);

	delete[] byteData;
}

/* The decoding is the same, but in reverse:
 * 1. Read the first three pixels to get the value of excessive bytes, say X
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

	if (byteData == nullptr)
		return;

	uintmax_t extraBytes = bytesToInt(byteData), byteDataSize = inputPNG.getImageSize();
	
	for (uintmax_t i = 9; i < (byteDataSize - extraBytes); i++)
		fputc(byteData[i], outputFile);

	delete[] byteData;
}

