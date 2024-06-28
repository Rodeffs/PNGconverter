#include "../inc/Converter.hpp"
#include "../inc/pngUtils.hpp"
#include <filesystem>
#include <exception>

using std::exception;
namespace fs = std::filesystem;

Converter::Converter(char* inFilePath, char* outFilePath) {

	inputFilePath = inFilePath;

	outFilePath = outFilePath;

	calculateInputFileSize();
}

void Converter::calculateInputFileSize() {

	try {
		inputFileSize = fs::file_size(inputFilePath);
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		return;
	}
}

void Converter::setInputFilePath(char* newInputFilePath) {

	inputFilePath = newInputFilePath;

	calculateInputFileSize();
}

void Converter::setOutputFilePath(char* newOutputFilePath) {

	outputFilePath = newOutputFilePath;
}

char* Converter::getInputFilePath() {

	return inputFilePath;
}

char* Converter::getOutputFilePath() {

	return outputFilePath;
}

unsigned char* Converter::intToBytes(uintmax_t value) {
	
	// Convert the unsigned int amount of extra bytes to bytes to be encoded into PNG

	unsigned char* convert = new unsigned char[8];

	uintmax_t temp = value;

	for (int i = 7; i >= 0; i--) {
		convert[i] = temp % 256;
		temp >>= 8;
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

unsigned char* Converter::readBytes(uintmax_t extraBytes) {

	FILE* inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error, unable to open input file");
		return nullptr;
	}

	unsigned char* data = new unsigned char[9 + inputFileSize + extraBytes];

	// Encoding first three pixels

	data[0] = 0;

	auto extraValues = intToBytes(extraBytes);

	for (int i = 0; i < 8; i++)
		data[i+1] = extraValues[i];

	delete[] extraValues;
	
	// Converting the initial data into byte data

	try {
		for (uintmax_t i = 0; i < inputFileSize; i++)
			data[i+9] = std::fgetc(inputFile); 
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		delete[] data;
		return nullptr;
	}
	
	// Adding extra bytes if there are any

	for (int i = 0; i < extraBytes; i++) 
		data[9 + inputFileSize + i] = 0;
	
	fclose(inputFile);

	return data;
} 

Resolution* Converter::bestResolution() {

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
	uintmax_t totalPixels = 3 + ceil(inputFileSize / 3);

	Resolution* resolution = new Resolution;

	resolution->height = floor(sqrt(totalPixels));

	resolution->width = resolution->height;

	int leftoverPixels = totalPixels - resolution->height * resolution->width;

	if (leftoverPixels > 0) {
		resolution->width += 1;
		leftoverPixels -= resolution->width;
	}

	if (leftoverPixels > 0)
		resolution->height += 1;

	return resolution;
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
	
	cout << "Finding the best resolution" << endl;

	auto resolution = bestResolution();

	// The amount of extra bytes that were added

	uintmax_t extraBytes = resolution->height * resolution->width * 3 - inputFileSize;

	cout << "Initializing PNG" << endl;

	ImagePNG outputPNG(outputFilePath, "wb");

	outputPNG.create(resolution->width, resolution->height);

	delete resolution;

	cout << "Reading byte data from input file" << endl;

	auto byteData = readBytes(extraBytes);

	cout << "Writing the data to PNG" << endl;

	outputPNG.write(byteData);
}

/* The decoding is the same, but in reverse:
 * 1. Read the first three pixels to get the value of excessive bytes, say X
 * 1. Continue reading the PNG pixel by pixel, row by row
 * 2. Every pixel color value is split into three bytes corresponding to the color
 * 3. Each byte is appended to output file
 * 4. The last X bytes are ignored and not added.
 */

void Converter::decode() {

}

