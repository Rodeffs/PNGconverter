#include "../inc/Converter.hpp"
#include <string>
#include <algorithm>
#include <iostream>

using std::find;
using std::string;
using std::stoi;
using std::cout;
using std::endl;

bool passed(char** begin, char** end, const string& parameter) {  // checks that the input parameter exists

	if (find(begin, end, parameter) != end)
		return true;

	return false;
}

char* parse(char** begin, char** end, const string& parameter) {  // gets the value of the input parameter

	char** iterator = find(begin, end, parameter);

	if (iterator != end && ++iterator != end) // ++ is needed to get the input after the parameter
		return *iterator;

	return 0;
}

int main(int argc, char *argv[]) {
	
	// If no parameters are given or switch --help is used

	if (argc == 1 || passed(argv, argv+argc, "--help")) {
		cout << "Usage:\n\n-e [file]\tencode the given file\n\n-d [file]\tdecode the given file\n\n-o [file]\toutput the encoded/decoded data to a given file\n\n-h [integer]\tthe height of the PNG to be encoded\n\n-w [integer]\tthe width of the PNG to be encoded\n\nNOTE: if no height or width were entered, the program will automatically pick the best resolution\n\n--help\t\tdisplay this help message" << endl;
		return 0;
	}

	// Obvious errors out of the way

	if (!passed(argv, argv+argc, "-o") || !(passed(argv, argv+argc, "-e") || passed(argv, argv+argc, "-d"))) {
		cout << "Error, not enough parameters were specified. Use --help to get help" << endl;
		return -1;
	}

	// Because both encoding and decoding require output file

	char* outputFilePath = parse(argv, argv+argc, "-o");

	if (!outputFilePath) {
		cout << "Error, no output file is given. Use -h or --help to get help" << endl;
		return -1;
	}

	Converter converter;

	// Parsing the specified parameters for decoding

	if (passed(argv, argv+argc, "-d")) {

		if (passed(argv, argv+argc, "-e") || passed(argv, argv+argc, "-h") || passed(argv, argv+argc, "-w")) {
			cout << "Error, conflicting parameters. Use --help to get help" << endl;
			return -1;
		}

		char* inputFilePath = parse(argv, argv+argc, "-d");

		if (!inputFilePath) {
			cout << "Error, no input file is given. Use --help to get help" << endl;
			return -1;
		}

		converter.setInputOutputFiles(inputFilePath, outputFilePath);

		converter.decode();

		return 0;
	}

	// Parsing the specified parameters for encoding

	char* inputFilePath = parse(argv, argv+argc, "-e");

	if (!inputFilePath) {
		cout << "Error, no input file is given. Use --help to get help" << endl;
		return -1;
	}

	converter.setInputOutputFiles(inputFilePath, outputFilePath);

	auto width = parse(argv, argv+argc, "-w");

	auto height = parse(argv, argv+argc, "-h");

	if (width && height)
		converter.setOutputImageResolution(stoi(width), stoi(height));
	
	converter.encode();

	return 0;
}
