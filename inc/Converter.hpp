#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <png.h>  // the libpng library used to read/write PNGs

using std::string;
using std::ifstream;
using std::ofstream;

enum class Action {CONVERT, DECONVERT};

class Converter {

private:
	ifstream* original;

public:
	Converter(ifstream& inputFile);

	void convert(Action action, ofstream& outputFile);


};
