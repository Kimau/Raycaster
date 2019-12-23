#include "filesys.h"

#include <stdio.h>

#pragma warning(disable : 4996)

std::vector<u8> ReadFileToBuffer(const char *filename) {
  // Open File
  FILE *pFile = fopen(filename, "rb");
  if (pFile == NULL)
    return std::vector<u8>();

  // Scan for Size
  fseek(pFile, 0, SEEK_END);
  long lSize = ftell(pFile);
  rewind(pFile);

  std::vector<u8> ttf_buffer(lSize);

  // Read into buffer
  bool success = (fread(&ttf_buffer[0], 1, ttf_buffer.size(), pFile) == lSize);
  fclose(pFile);

  ttf_buffer.push_back(0);
  return ttf_buffer;
}
