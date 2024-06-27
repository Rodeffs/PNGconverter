#include "../inc/pngUtils.hpp"

int pngUtils::checkIfPng(FILE* openedFile) {
	unsigned char buffer[8];

	if (fread(buffer, 1, 8, openedFile) != 8)
		return 0;

	return png_sig_cmp(buffer, 0, 8) == 0;  // read the first 8 bytes of the signature and return true if they match
}
