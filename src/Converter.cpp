#include "../inc/Converter.hpp"
#include <cstdint>
#include <cstdio>
#include <exception>
#include <png.h>
#include <pngconf.h>

Converter::Converter(char* inputFilePath, char* outputFilePath) {

	inputFile = fopen(inputFilePath, "rb");

	outputFile = fopen(outputFilePath, "wb");

	try {
		inputFileSize = std::filesystem::file_size(inputFilePath);  // since we need FileSize later
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

unsigned char* Converter::readOriginal() {

	unsigned char* buffer = new unsigned char[inputFileSize];
	
	try {
		for (uintmax_t i = 0; i < inputFileSize; i++)
			buffer[i] = std::fgetc(inputFile); 
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		delete[] buffer;
		return nullptr;
	}

	fclose(inputFile);

	return buffer;
} 

/* So the way I plan to convert the file to PNG is this:
 * 1. The resolution of the output PNG is set to X by X pixels, where X is the ceil(sqrt(original file size / 3))
 * 2. Each byte from the original file is converted into a RGB value (from 0 to 255).
 * 3. The very first pixel at the beginning stores information about whether or not the initial file size was divisible by 3.
 * 3.1. This is because if there aren't enough bytes to group together, then the placeholder value of 0 is used for the remaining colors
 * 3.2. To make the decoding unambiguous we need to know whether or not these values were there in the original file or not
 * 3.3. To do that, use the formula = (3 - original file size % 3) % 3
 * 3.4. If output is 2, then the RGB value of the first pixel is (0, 255, 255)
 * 3.5. If output is 1, then it's (0, 0, 255)
 * 3.6. If output is 0, then it's (0, 0, 0)
 * 4. Every 3 encoded bytes are then grouped together to form an RGB value for every pixel.
 * 5. If there are remaining uncoded pixels in the PNG, then they are just transparent with RGB values of (0, 0, 0)
 */

void Converter::encode() {

	if (!checkFiles())
		return;
	
	unsigned int resolution = ceil(sqrt(inputFileSize));
	
	// Original file buffer

	auto buffer = readOriginal();

	// The fun begins now
	
	// A pointer to the resulting PNG

	png_structp resultingPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	// This is the struct that hold pixel data

	png_infop infoCurrentPNG = png_create_info_struct(resultingPNG);

	// Error handling
	
	if (setjmp(png_jmpbuf(resultingPNG))) {
		png_destroy_write_struct(&resultingPNG, &infoCurrentPNG);
		cout << "Error when modifying PNG to write" << endl;
		return;
	}

	// This is the output code

	png_init_io(resultingPNG, outputFile);

	// This is all the information about the PNG, its resolution (which we calculated earlier), bit depth, color type, interlace, compression type and filter type
	
	png_set_IHDR(resultingPNG, infoCurrentPNG, resolution, resolution, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	// Write to image row by row

	for (unsigned int y = 0; y < resolution; y++) {
		auto row = new png_bytep[3 * resolution * sizeof(png_byte)];

		for (unsigned int x = 0; x < resolution; x++) {

		}
	}
}

/* The decoding is the same, but in reverse:
 * 1. Read the first pixel, remember its value
 * 1. Continue reading the PNG pixel by pixel, row by row
 * 2. Every pixel color value is split into three bytes corresponding to the color
 * 3. Each byte is appended to output file
 * 4. If the first pixel value is (0, 255, 255) last two bytes are ignored and not appended
 * 4.1. If it's (0, 0, 255) then the last byte is ignored
 * 4.2. If it's (0, 0, 0) then no bytes are ignored
 * 5. If a pixel is transparent, then it's ignored and not appended
 */

void Converter::decode() {

	if (!checkFiles())
		return;

	if (!pngUtils::checkIfPng(inputFile)) {
		cout << "Error, the input file is not a PNG" << endl;
		return;
	}

}

