#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <msfl.hpp>

struct png_file {
	byte* data;
	size_t sz;
	size_t width = 0;
	size_t height = 0;
	int channels = 0;
	std::string src;
};

struct pngInfo {
	int width = 0;
	int height = 0;
	int bitDepth = 8;
	int filteringMethod = 0;
};

class PngParse {
public:
	static png_file Decode(std::string src);
	static png_file DecodeBytes(byte* bytes, size_t sz);
	static bool WriteToFile(std::string src, byte* dat, size_t sz, pngInfo pInf);
};