#include "gltex.h"

#include "libs/sdl/include/SDL.h"

#define STBI_ASSERT(x) SDL_assert(x)
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "libs/stb/stb_image.h"
#include "libs/stb/stb_image_write.h"

ImageData::ImageData(const char *friendlyName)
    : name(friendlyName), loaded(false), width(-1), height(-1),
      imgType(GL_RGBA), glTex(-1) {}

void ImageData::LoadFromFile(const char *filename) {
	FreeImage();

  int depth;
  u8 *data = stbi_load(filename, &width, &height, &depth, 0);

  if (data) {
    loaded = true;
    imgData = data;

    if (depth == 3)
      imgType = GL_RGB;
    else
      imgType = GL_RGBA;
  }
}

int ImageData::SaveFile(const char *filename) {
	stbi_flip_vertically_on_write(true);

	if (imgType == GL_RGB)
		return stbi_write_png(filename, width, height, 3, imgData, 0);
	else if (imgType == GL_RGBA)
		return stbi_write_png(filename, width, height, 4, imgData, 0);
	else
		return 0;
}

void ImageData::BlankImage(int _width, int _height) {
	FreeImage();

  width = _width;
  height = _height;
  imgType = GL_RGB;

  loaded = true;
  imgData = (u8*)stbi__malloc(width * height * 3);
}

bool ImageData::CopyToGPU() {
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
    printf("\n BindGLTex %s Data Error %04X \n %dx%d %d", name, e, width,
           height, imgType);
    return false;
  }

  return true;
}

void ImageData::FreeImage() {
  if (!loaded)
    return;

  glTexImage2D(GL_TEXTURE_2D, 0, imgType, width, height, 0, imgType, GL_UNSIGNED_BYTE, NULL);
  stbi_image_free(imgData);
  loaded = false;
}

void ImageData::BindTex() {
  if (false == IsBindable())
    return;

  glBindTexture(GL_TEXTURE_2D, glTex);
}
