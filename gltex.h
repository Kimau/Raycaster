#pragma once

#include <GL/gl3w.h>

struct ImageData
{
	bool loaded;
	int width, height, imgType;
	GLuint glTex;
	const char* name;
	unsigned char *imgData;

	ImageData(const char* friendlyName);;

	void LoadFromFile(const char* filename);
	bool CopyToGPU();
	void FreeImage();

	void BindTex();
};
