#include "../inc/pngUtilities.hpp"

using namespace pngUtilities;

int checkIfPng(FILE** file) {
	
	char buffer[8];

	if (fread(buffer, 1, 8, *file) != 8)
		return 0;

	return png_sig_cmp(buffer, 0, 8) == 0;  // read the first 8 bytes of the signature and return true if they match
}
