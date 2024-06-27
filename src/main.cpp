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

	if (argc == 1 || parameterExists(argv, argv+argc, "--help")) {
		cout << "Usage:\n\n-i [file]\tinput file\n\n-o [file]\toutput file\n\n-c\t\tconvert, assumed by default\n\n-d\t\tdeconvert\n\n--help\t\tdisplay this help message\n\nNote that only the FIRST instances of -i and -o are read\n" << endl;
		return 0;
	}
	
	// Parsing input file

	char* inputFilePath = parseInput(argv, argv+argc, "-i");

	if (!inputFilePath) {
		cout << "Error, no input file is given. Use --help to get help" << endl;
		return 0;
	}

	// Parsing output file

	char* outputFilePath = parseInput(argv, argv+argc, "-o");

	if (!outputFilePath) {
		cout << "Error, no output file is given. Use --help to get help" << endl;
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

	// Now pass all the parameters to the class and begin conversion
	
	Converter converter(inputFilePath, outputFilePath);

	converter.execute(action);

	return 0;
}
