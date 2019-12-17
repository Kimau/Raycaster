#include "filesys.h"

#include <stdio.h>

#pragma warning( disable : 4996 ) 

std::vector<unsigned char> ReadFileToBuffer(const char* filename)
{
	// Open File
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL)
		return std::vector<unsigned char>();

	// Scan for Size
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);

	std::vector<unsigned char> ttf_buffer(lSize);

	// Read into buffer
	bool success = (fread(&ttf_buffer[0], 1, ttf_buffer.size(), pFile) == lSize);
	fclose(pFile);

	return ttf_buffer;
}

