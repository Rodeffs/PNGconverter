#pragma once

#include "libraries.hpp"

namespace pngUtilities {
	
	int checkIfPng(FILE** file);  // read the first 8 bytes of a file to check if it's PNG
}
