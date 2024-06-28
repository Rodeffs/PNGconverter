#include "../inc/pngUtils.hpp"

ImagePNG::ImagePNG(char* filePath, const char* mode) {

	if (strcmp(mode, "wb"))
		accessType = Access::WRITE;
	else if (strcmp(mode, "rb"))
		accessType = Access::READ;

	currentFile = fopen(filePath, mode);

	if (!checkCurrentFile())
		return;

	// A pointer to the resulting PNG

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	// This is the struct that hold pixel data

	info_ptr = png_create_info_struct(png_ptr);

	// Error handling
	
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		cout << "Error when modifying PNG" << endl;
		return;
	}

	// Path to the PNG

	png_init_io(png_ptr, currentFile);
}

ImagePNG::~ImagePNG() {

	if (currentFile != NULL)
		fclose(currentFile);

	if (info_ptr != NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

	if (png_ptr != NULL)
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

bool ImagePNG::checkCurrentFile() {

	if (!currentFile) {
		perror("Error, unable to open input file");
		return false;
	}

	return true;
}

int ImagePNG::checkIfPng() {

	if (!checkCurrentFile())
		return -1;

	unsigned char buffer[8];

	if (fread(buffer, 1, 8, currentFile) != 8)
		return 0;

	return png_sig_cmp(buffer, 0, 8) == 0;  // read the first 8 bytes of the signature and return true if they match
}

void ImagePNG::create(uintmax_t width, uintmax_t height) {

	if (accessType == Access::READ) {
		cout << "Error, can't create PNG because it's opened in read mode" << endl;
		return;
	}

	if (isCreated) {
		cout << "Error, PNG is already created" << endl;
		return;
	}

	// IHDR chunk store all the necessary info about the PNG

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	isCreated = true;
}

void ImagePNG::write(unsigned char* data) {

	// Write to image row by row

	for (unsigned int y = 0; y < height; y++) {
		auto row = new png_bytep[3 * width * sizeof(png_byte)];

		for (unsigned int x = 0; x < width; x++) {

		}
	}


}

unsigned char* ImagePNG::read() {

	return nullptr;
}
