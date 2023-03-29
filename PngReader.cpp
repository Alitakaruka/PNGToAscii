#include "PngReader.h"
#include <vector>
#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif
#pragma region Constructor

char ascii[18] = " .:!/r(l1Z4H9W8$@";
PngReader::~PngReader()
{
}

void PngReader::readIHDR(std::ifstream* file)
{
	char byte;
	//idat len
	for (int i = 1; i <= 4; i++) {
		file->read(&byte, sizeof(byte));
	}

	//name
	for (int i = 1; i <= 4; i++) {
		file->read(&byte, sizeof(byte));
	}
	//data
	char buffer[13];
	file->read(buffer, sizeof(buffer));
	unsigned char* data = reinterpret_cast<unsigned char*>(buffer);

	//pngWidth
	for (int i = 0; i < 3; i++) {
		this->pngWidth = (data[i] << 8) | data[i + 1];
	}

	//pngHeight
	for (int i = 4; i < 7; i++) {
		this->pngHeight += (int)(data[i] << 8) | data[i + 1];
	}

	this->bitsBufer = (int)data[8];

	std::cout << buffer;
}

void PngReader::readSignature() {
	std::ifstream file;

	file.open(this->path);
	if (!file.is_open()) {
		this->isPng = false;
		return;
	}
	char byte[8];

	file.read(byte, sizeof(byte));

	this->isPng = (int)byte[0] == -119 && byte[1] == 'P' && byte[2] == 'N' && byte[3] && 'G';
#ifdef DEBUG
	std::cout << byte;
#endif // DEBUG

	file.close();

}

void PngReader::readSignature(char* path)
{
	std::ifstream file(path, std::ios::binary);

	file.open(this->path);
	if (!file.is_open()) {
		this->isPng = false;
		return;
	}
	char byte[8];

	file.read(byte, sizeof(byte));

	this->isPng = byte[1] == 'P' && byte[2] == 'N' && byte[3] && 'G';
#ifdef DEBUG
	std::cout << byte;
#endif // DEBUG

	file.close();
}
#pragma endregion

#pragma region Pngreader

PngReader::PngReader()
{
}

PngReader::PngReader(char* path)
{
	this->path = path;
	readSignature();
}
#pragma endregion

#pragma region is_png
bool PngReader::is_png()
{
	return isPng;
}

bool PngReader::is_png(char* path)
{
	return false;
}

bool PngReader::is_png(std::string path)
{
	return false;
}
#pragma endregion

char get_ascii(unsigned char* ch) {
	int light = ( * ch + *(ch + 1) + *(ch + 2))/2 / std::size(ascii);
	if (std::size(ascii)< light) {
		light = std::size(ascii) - 1;
	}
	return ascii[light];
}
bool PngReader::to_ascii_graph()
{
	if (!this->isPng) {
		return false;
	}
	std::ifstream file;
	file.open(this->path, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}
	std::ofstream writeTxtFile("AsiiScreen.txt");
	char buffer[8];
	//skip read png
	file.read(buffer, sizeof(buffer));
	//
	//chunk IHDR
	readIHDR(&file);
	//
	//read pixels 
	char bufferRGB[3];
	for (int i = 0; i < this->pngWidth;i++) {
		for (int j = 0; j < this->pngHeight;j++) {
			file.read(buffer, sizeof(buffer));
			unsigned char* data = reinterpret_cast<unsigned char*>(buffer);
			writeTxtFile << get_ascii(data);
		}
		writeTxtFile <<'\n';
	}
	writeTxtFile.close();
	file.close();
	return false;
}
