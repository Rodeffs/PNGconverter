#include "../inc/Converter.hpp"

Converter::Converter(char* inFilePath, char* outFilePath) {

	inputFilePath = inFilePath;

	outFilePath = outFilePath;

	try {
		inputFileSize = std::filesystem::file_size(inputFilePath);  // since we need FileSize later
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		return;
	}
}

unsigned char* Converter::readBytes(int extraBytes) {

	FILE* inputFile = fopen(inputFilePath, "rb");

	if (!inputFile) {
		perror("Error, unable to open input file");
		return nullptr;
	}

	unsigned char* data = new unsigned char[3 + inputFileSize];

	// The very first pixel values as discussed below

	data[0] = 0;

	data[1] = (extraBytes == 2) ? 255 : 0;

	data[2] = (extraBytes != 0) ? 255 : 0;
	
	try {
		for (uintmax_t i = 0; i < inputFileSize; i++)
			data[i+3] = std::fgetc(inputFile); 
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
		delete[] data;
		return nullptr;
	}

	fclose(inputFile);

	return data;
} 

/* So the way I plan to convert the file to PNG is this:
 * 1. The resolution of the output PNG is set to X by X pixels, where X is the ceil(sqrt(original file size / 3))
 * 2. Each byte from the original file is converted into a RGB value (from 0 to 255).
 * 3. The very first pixel at the beginning stores information about whether or not the initial file size was divisible by 3.
 * 3.1. This is because if there aren't enough bytes to group together, then the placeholder value of 0 is used for the remaining colors
 * 3.2. To make the decoding unambiguous we need to know whether or not these values were there in the original file or not
 * 3.3. To do that, use the formula = (3 - original file size % 3) % 3
 * 3.4. If output is 2, then the RGB value of the first pixel is (0, 255, 255)
 * 3.5. If output is 1, then it's (0, 0, 255)
 * 3.6. If output is 0, then it's (0, 0, 0)
 * 4. Every 3 encoded bytes are then grouped together to form an RGB value for every pixel.
 * 5. If there are remaining uncoded pixels in the PNG, then they are just transparent with RGB values of (0, 0, 0)
 */

void Converter::encode() {

	uintmax_t resolution = ceil(sqrt(1 + inputFileSize / 3));
	
	ImagePNG outputPNG(outputFilePath, "wb");

	outputPNG.create(resolution, resolution);

	int extraBytes = (3 - inputFileSize % 3) % 3;

	outputPNG.write(readBytes(extraBytes));
}

/* The decoding is the same, but in reverse:
 * 1. Read the first pixel, remember its value
 * 1. Continue reading the PNG pixel by pixel, row by row
 * 2. Every pixel color value is split into three bytes corresponding to the color
 * 3. Each byte is appended to output file
 * 4. If the first pixel value is (0, 255, 255) last two bytes are ignored and not appended
 * 4.1. If it's (0, 0, 255) then the last byte is ignored
 * 4.2. If it's (0, 0, 0) then no bytes are ignored
 * 5. If a pixel is transparent, then it's ignored and not appended
 */

void Converter::decode() {

}

