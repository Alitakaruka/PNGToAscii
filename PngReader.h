#pragma once
#include<fstream>
class PngReader
{
public:
	PngReader();
	PngReader(char* path);
	PngReader(std::string path);
	~PngReader();


	bool is_png();
	bool is_png(char* path);
	bool is_png(std::string path);

	bool to_ascii_graph();

private:
	int bitsBufer;
	char* path = nullptr;
	int pngWidth;
	int pngHeight;
	bool isPng = false;

private:
	void readIHDR(std::ifstream* file);
	void readSignature();
	void readSignature(char* file);
	
};

