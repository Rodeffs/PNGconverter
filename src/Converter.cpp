#include "../inc/Converter.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>

Converter::Converter(ifstream& inputFile) {
	original = &inputFile;

}

void Converter::convert(Action action, ofstream& outputFile) {
	string line;

	while ((*original).good()) {
		getline(*original, line);
		std::cout << line << std::endl;
	}

	outputFile << "testing";
}
