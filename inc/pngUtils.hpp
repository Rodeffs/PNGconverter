#pragma once

#include <cstdio>
#include <png.h>  // the libpng library used to read/write PNGs

namespace pngUtils {
	
	int checkIfPng(FILE* openedFile); // works with already opened files
	
}
