#include "../inc/libraries.hpp"
#include "../inc/Converter.hpp"

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
		cout << "Usage:\n\n-i [file] or --input-file [file]\tselect the input file to read from\n\n-o [file] or --output-file [file]\tselect the output file to write to\n\n-e or --encode\t\t\t\tencoding, assumed by default\n\n-d or --decode\t\t\t\tdecoding\n\n-g or --grayscale\t\t\tif used for encoding, will make the PNG black & white\n\n-h or --help\t\t\t\tdisplay this help message" << endl;
		return 0;
	}
	
	// Parsing the input file

	char* inputFilePath = parseInput(argv, argv+argc, "-i");

	if (!inputFilePath)
		inputFilePath = parseInput(argv, argv+argc, "--input-file");

	if (!inputFilePath) {
		cout << "Error, no input file is given. Use --help to get help" << endl;
		return 0;
	}

	// Parsing the output file

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

	if (parameterExists(argv, argv+argc, "-d") || parameterExists(argv, argv+argc, "--decode")) {
		if (parameterExists(argv, argv+argc, "-e") || parameterExists(argv, argv+argc, "--encode")) {
			cout << "Error, conflicting switches. Use either -e or -d" << endl;
			return 0;
		}
		converter.decode();
	}
	else {
		if (parameterExists(argv, argv+argc, "-g") || parameterExists(argv, argv+argc, "--grayscale"))
			converter.setGrayscale(true);
		
		converter.encode();
	}

	return 0;
}
