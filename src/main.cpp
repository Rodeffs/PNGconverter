#include <filesystem>
#include <iostream>
#include <algorithm>
#include "../inc/Converter.hpp"

using std::cout;
using std::endl;
using std::find;

bool parameterExists(char** begin, char** end, const string& parameter) {  // checks that the input parameter exists

	if (find(begin, end, parameter) != end)
		return true;

	return false;
}

char* parseInput(char** begin, char** end, const string& parameter) {  // gets the value of the input parameter

	char** iterator = find(begin, end, parameter);

	if (iterator != end && ++iterator != end) // ++ is needed to get the INPUT after the parameter
		return *iterator;

	return 0;
}

int main(int argc, char *argv[]) {
	
	// If no parameters are given or switch --help is used

	if (argc == 1 || parameterExists(argv, argv+argc, "--help")) {
		cout << "Usage:\n\n-i [file]\tinput file\n\n-o [file]\toutput file, by default for conversion it is [input file].png and for deconversion it is [input file]_original in the current working directory\n\n-c\t\tconvert, assumed by default if -i is given\n\n-d\t\tdeconvert\n\n--help\t\tdisplay this help message\n\nNote that only the FIRST instances of -i or -o are read\n" << endl;
		return 0;
	}
	
	// Reading input file

	char* inputFileName = parseInput(argv, argv+argc, "-i");

	if (!inputFileName) {
		cout << "Error, no input file is given. Use --help to get help" << endl;
		return 0;
	}

	ifstream inputFile(inputFileName);

	if (!inputFile.is_open()) {
		cout << "Error, unable to open file" << endl;
		return 0;
	}

	// Checking whether to convert or to deconvert the input file, conversion is the default

	Action action;

	if (parameterExists(argv, argv+argc, "-d")) {
		if (parameterExists(argv, argv+argc, "-c")) {
			cout << "Error, conflicting switches. Use either -c or -d" << endl;
			return 0;
		}
		action = Action::DECONVERT;
	}
	else
		action = Action::CONVERT;

	// Writing to output file, by default it is inputFileName.png for conversion and inputFileName_original for deconversion in the current working directory

	char* pathString = parseInput(argv, argv+argc, "-o");

	auto outputFilePath = std::filesystem::current_path(); // to get current working directory

	string defaultFileName = inputFileName;
	
	if (action == Action::CONVERT)
		defaultFileName.append(".png");
	else
		defaultFileName.append("_original");

	if (pathString) {
		std::filesystem::path userPath(pathString);

		if (std::filesystem::is_directory(userPath))
			outputFilePath = (userPath / defaultFileName);
		else if (std::filesystem::is_regular_file(userPath))
			outputFilePath = userPath;
	}
	else
		outputFilePath /= defaultFileName;

	ofstream outputFile(outputFilePath);

	if (!outputFile.is_open()) {
		cout << "Error, unable to write to a file" << endl;
		return 0;
	}

	// Now pass all the parameters to the class and begin conversion
	
	Converter picture(inputFile);

	picture.convert(action, outputFile);

	// Never forget to close the files!

	inputFile.close();

	outputFile.close();

	return 0;
}
