#include "gltex.h"

#include "libs/sdl/include/SDL.h"

#define STBI_ASSERT(x) SDL_assert(x)
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"

ImageData::ImageData(const char* friendlyName) : name(friendlyName), loaded(false),
width(-1), height(-1), imgType(GL_RGBA), glTex(-1)
{

}

void ImageData::LoadFromFile(const char* filename)
{
	int depth;
	unsigned char *data = stbi_load(filename, &width, &height, &depth, 0);

	if (data) {
		loaded = true;
		imgData = data;

		if (depth == 3) imgType = GL_RGB;
		else imgType = GL_RGBA;
	}
}

bool ImageData::CopyToGPU()
{
	if (!loaded)
		return false;

	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int e = glGetError();
	if (e != GL_NO_ERROR) {
		printf("\n BindGLTex %s Setup Error %04X \n", name, e);
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, imgType, width, height, 0, imgType, GL_UNSIGNED_BYTE, imgData);

	e = glGetError();
	if (e != GL_NO_ERROR) {
		printf("\n BindGLTex %s Data Error %04X \n %dx%d %d", name, e, width, height, imgType);
		return false;
	}

	return true;
}

void ImageData::FreeImage()
{
	if (!loaded)
		return;

	stbi_image_free(imgData);
	loaded = false;
}

void ImageData::BindTex()
{
	glBindTexture(GL_TEXTURE_2D, glTex);
}
