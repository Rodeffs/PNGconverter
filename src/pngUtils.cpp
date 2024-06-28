#include "../inc/pngUtils.hpp"
#include <cstdint>
#include <iostream>
#include <png.h>
#include <pngconf.h>

using std::cout;
using std::endl;
using std::perror;

uintmax_t ImagePNG::getReadDataSize() {

	return readDataSize;
}

int ImagePNG::checkIfPNG(FILE *file) {

	unsigned char buffer[8];

	if (fread(buffer, 1, 8, file) != 8)
		return 0;

	return png_sig_cmp(buffer, 0, 8) == 0;  // read the first 8 bytes of the signature and return true if they match
}


void ImagePNG::write(unsigned char* data, FILE* outputFile, uintmax_t width, uintmax_t height, uintmax_t dataSize) {

	if (!outputFile) {
		perror("Error in pngUtils, unable to open output file");
		return;
	}

	// A pointer to the resulting PNG

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	// info_ptr stores all the information about the PNG

	info_ptr = png_create_info_struct(png_ptr);

	// Error handling
	
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		cout << "Error while writing to PNG" << endl;
		return;
	}

	// Path to the PNG

	png_init_io(png_ptr, outputFile);
	
	// IHDR chunk stores all the necessary info about the PNG

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	
	// Don't forget to write all chunks before writing pixel data!

	png_write_info(png_ptr, info_ptr);

	// Writing to image row by row
	// Y'know, everything would be much easier if there was actual documentation for that, not just
	// plaintext with a couple of examples that don't really explain much

	png_bytep row = new png_byte[3 * width * sizeof(png_byte)];
	
	for (uintmax_t y = 0; y < height; y++) {
		for (uintmax_t x = 0; x < width*3; x++)
			row[x] = data[y*height*3 + x];
		png_write_row(png_ptr, row);
	}

	// Getting rid of the data

	png_write_end(png_ptr, NULL);

	delete[] row;

	png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

unsigned char* ImagePNG::read(FILE* inputFile) {

	if (!inputFile) {
		perror("Error in pngUtils, unable to open input file");
		return nullptr;
	}

	if (!checkIfPNG(inputFile)) {
		cout << "Error, the input file is not a PNG";
		return nullptr;
	}
	
	// A pointer to the existing PNG

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);

	// Error handling
	
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		cout << "Error while reading PNG" << endl;
		return nullptr;
	}

	png_init_io(png_ptr, inputFile);
	
	// Since we already read the first 8 bytes to check if it's a PNG, skip them

	png_set_sig_bytes(png_ptr, 8);
	
	// Reading info about the PNG

	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;

	int bit_depth, color_type, interlace_method;

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_method, NULL, NULL);
	
	// Checking if it was encoded with the needed parameters

	if ((bit_depth != 8) || (width > 1000000) || (height > 1000000) || (color_type != PNG_COLOR_TYPE_RGB) || (interlace_method != PNG_INTERLACE_NONE)) {
		cout << "Error, this PNG can't be decoded" << endl;
		return nullptr;
	}
	
	// Getting pixel value data
	
	readDataSize = height * width * 3;	

	png_bytepp row_pointers = new png_bytep[height * sizeof(png_bytep)];

	for (int i = 0; i < height; i++)
		row_pointers[i] = new png_byte[width * 3 * sizeof(png_byte)];

	png_set_rows(png_ptr, info_ptr, row_pointers);

	png_read_image(png_ptr, row_pointers);
	
	// Converting the pixel data to byte data
	
	unsigned char* byteData = new unsigned char[readDataSize];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width*3; x++)
			byteData[y*height*3 + x] = row_pointers[y][x];
	}

	// Getting rid of the data
	
	for (int i = 0; i < height; i++)
		delete[] row_pointers[i];

	delete[] row_pointers;

	png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

	png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

	return byteData;
}
