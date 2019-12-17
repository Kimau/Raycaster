#include "core.h"
#include "program.h"
#include "gltex.h"

void Raycast(ImageData& output) {

	float invWidth = 1.0f / float(output.width);
	float invHeight = 1.0f / float(output.height);

	int c = 0;
	for (int y = output.height - 1; y >= 0; --y) {
		for (int x = output.width - 1; x >= 0; --x) {		
			output.imgData[c++] = unsigned char(255 * x * invWidth);
			output.imgData[c++] = unsigned char(255 * y * invHeight);
			output.imgData[c++] = unsigned char(255 * 0.2f);
		}
	}

}