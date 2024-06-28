#include "../inc/Converter.hpp"

Converter::Converter(char* inFilePath, char* outFilePath) {

	inputFilePath = inFilePath;

	outFilePath = outFilePath;

	calculateInputFileSize();
}

void Converter::calculateInputFileSize() {

	try {
		inputFileSize = std::filesystem::file_size(inputFilePath);
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		return;
	}
}

void Converter::setInputFilePath(char* newInputFilePath) {

	inputFilePath = newInputFilePath;

	calculateInputFileSize();
}

void Converter::setOutputFilePath(char* newOutputFilePath) {

	outputFilePath = newOutputFilePath;
}

char* Converter::getInputFilePath() {

	return inputFilePath;
}

char* Converter::getOutputFilePath() {

	return outputFilePath;
}

unsigned char* Converter::readBytes(int extraBytes) {

	FILE* inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error, unable to open input file");
		return nullptr;
	}

	unsigned char* data = new unsigned char[3 + inputFileSize + extraBytes];

	// The very first pixel values as discussed below

	data[0] = 0;

	data[1] = (extraBytes == 2) ? 255 : 0;

	data[2] = (extraBytes != 0) ? 255 : 0;
	
	// Converting the initial data into byte data

	try {
		for (uintmax_t i = 0; i < inputFileSize; i++)
			data[i+3] = std::fgetc(inputFile); 
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		delete[] data;
		return nullptr;
	}
	
	// Adding extra bytes if there are any

	for (int i = 0; i < extraBytes; i++) 
		data[3 + inputFileSize + i] = 0;
	
	fclose(inputFile);

	return data;
} 

/* So the way I plan to convert the file to PNG is this:
 * 1. The resolution of the output PNG is set to X by X pixels, where X is the ceil(sqrt(2 + original file size / 3))
 * 2. The first three pixels (72 bits of data) are reserved and store information about the excessive bytes:
 * 	2.1. First, calculate value of (3 - original file size % 3) % 3, so that the last pixel of actual data has 3 bytes and not 2 or 1
 * 	2.2. Then calculate X - original file size
 * 	2.3. Add those values together to get a number of extra bytes in the image
 * 	2.4. Store this value in the first three pixels.
 * 3. Each byte from the original file is converted into a RGB value (from 0 to 255).
 * 4. Every 3 encoded bytes are then grouped together to form an RGB value for every pixel.
 * 5. All extra bytes are always 0
 *
 * Since unsigned int size is 64 bits (8 bytes) for most platforms and a pixel is encoded with 24 bits (3 bytes) of data, I had to reserved
 * the first 3 pixels (72 bits = 9 bytes) to store that info, which means that the first byte of the first reserved pixel will always be 0
 * As I don't really know how to encode transparency with this library (and TBH don't want to know) and using alpha-channels would double
 * the PNG size, I think this solution is good enough to store info about excessive bytes.
 * It would only fail in case the amount of excess data is 2^64 bytes, that would require the initial file size prior to conversion to be
 * at least 2^64 * (2^64 - 1) bytes, which is more than 2^27 quettabytes of data and I think that it's decent enough for my project 
 */

void Converter::encode() {
	
	cout << "Configuring PNG resolution" << endl;

	uintmax_t resolution = ceil(sqrt(1 + inputFileSize / 3));

	cout << "Initializing PNG" << endl;

	ImagePNG outputPNG(outputFilePath, "wb");

	outputPNG.create(resolution, resolution);

	cout << "Reading byte data from input file" << endl;

	int extraBytes = (3 - inputFileSize % 3) % 3;

	auto byteData = readBytes(extraBytes);

	cout << "Writing the data to PNG" << endl;

	outputPNG.write(byteData);
}

/* The decoding is the same, but in reverse:
 * 1. Read the first three pixels to get the value of excessive bytes, say X
 * 1. Continue reading the PNG pixel by pixel, row by row
 * 2. Every pixel color value is split into three bytes corresponding to the color
 * 3. Each byte is appended to output file
 * 4. The last X bytes are ignored and not added.
 */

void Converter::decode() {

}

