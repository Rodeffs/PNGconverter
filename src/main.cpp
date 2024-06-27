#include "../inc/libraries.hpp"
#include "../inc/Converter.hpp"
#include <cstdlib>

bool parameterExists(char** begin, char** end, const string& parameter) {  // checks that the input parameter exists

	if (find(begin, end, parameter) != end)
		return true;

	return false;
}

char* parseInput(char** begin, char** end, const string& parameter) {  // gets the value of the input parameter

	char** iterator = find(begin, end, parameter);

	if (iterator != end && ++iterator != end) // ++ is needed to get the input after the parameter
		return *iterator;

	return 0;
}

int main(int argc, char *argv[]) {
	
	// If no parameters are given or switch --help is used

	if (argc == 1 || parameterExists(argv, argv+argc, "--help") || parameterExists(argv, argv+argc, "-h")) {
		cout << "Usage:\n\n-i [file] or --input-file [file]\tselect the input file to read from\n\n-o [file] or --output-file [file]\tselect the output file to write to\n\n-c or --convert\t\t\t\tconversion, assumed by default\n\n-d or --deconvert\t\t\tdeconversion\n\n-g or --grayscale\t\t\tif used for conversion, will make the PNG grayscale\n\n-p or --pixel-depth\t\t\tselects the pixel depth, available values: 8 and 16 bits for grayscale; 8, 24 and 48 bits otherwise, the default is 8 bits\n\n-h or --help\t\t\t\tdisplay this help message\n\nNote that only the FIRST instances of -i and -o are read\n" << endl;
		return 0;
	}
	
	// Parsing input file

	char* inputFilePath = parseInput(argv, argv+argc, "-i");

	if (!inputFilePath)
		inputFilePath = parseInput(argv, argv+argc, "--input-file");

	if (!inputFilePath) {
		cout << "Error, no input file is given. Use --help to get help" << endl;
		return 0;
	}

	// Parsing output file

	char* outputFilePath = parseInput(argv, argv+argc, "-o");

	if (!outputFilePath)
		outputFilePath = parseInput(argv, argv+argc, "--output-file");

	if (!outputFilePath) {
		cout << "Error, no output file is given. Use -h or --help to get help" << endl;
		return 0;
	}

	// Now pass the filenames to Converter

	Converter converter(inputFilePath, outputFilePath);

	// Checking whether to convert or to deconvert the input file, conversion is the default

	if (parameterExists(argv, argv+argc, "-d")) {
		if (parameterExists(argv, argv+argc, "-c")) {
			cout << "Error, conflicting switches. Use either -c or -d" << endl;
			return 0;
		}
		converter.deconvert();
	}
	else {
		if (parameterExists(argv, argv+argc, "-g") || parameterExists(argv, argv+argc, "--grayscale"))
			converter.setGrayscale(true);
		
		char* pixelDepth = parseInput(argv, argv+argc, "-p");

		if (!pixelDepth)
			pixelDepth = parseInput(argv, argv+argc, "--pixel-depth");

		if (pixelDepth)
			converter.setPixelDepth(std::atoi(pixelDepth));

		converter.convert();
	}

	return 0;
}
