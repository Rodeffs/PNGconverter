#include "../inc/pngUtils.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>

using std::cout;
using std::endl;
using std::perror;

ImagePNG::ImagePNG() {

	// A pointer to the resulting PNG

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	// This is the struct that hold pixel data

	info_ptr = png_create_info_struct(png_ptr);

	// Error handling idk if incorrect
	
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		cout << "Error when modifying PNG" << endl;
		return;
	}
}

ImagePNG::~ImagePNG() {

	if (info_ptr != NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

	if (png_ptr != NULL)
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

uintmax_t ImagePNG::getReadDataSize() {

	return readDataSize;
}

int ImagePNG::checkIfPng(FILE *file) {

	unsigned char buffer[8];

	if (fread(buffer, 1, 8, file) != 8)
		return 0;

	return png_sig_cmp(buffer, 0, 8) == 0;  // read the first 8 bytes of the signature and return true if they match
}


void ImagePNG::write(unsigned char* data, FILE* outputFile, uintmax_t width, uintmax_t height) {

	if (!outputFile) {
		perror("Error in pngUtils, unable to open output file");
		return;
	}

	// Path to the PNG

	png_init_io(png_ptr, outputFile);
	
	// IHDR chunk store all the necessary info about the PNG

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	// Don't forget to write all chunks before writing pixel data!

	png_write_info(png_ptr, info_ptr);

	// Writing to image row by row
	// Y'know, everything would be much easier if there was actual documentation for that, not just
	// plaintext with a couple of examples that don't really explain much

	png_bytep row = new png_byte[3 * width * sizeof(png_byte)];

	int i = 0;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width*3; x++) {
			row[x] = data[i];
			i++;
		}
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, NULL);

	delete[] row;
}

unsigned char* ImagePNG::read(FILE* inputFile) {

	if (!inputFile) {
		perror("Error in pngUtils, unable to open input file");
		return nullptr;
	}

	return nullptr;
}
