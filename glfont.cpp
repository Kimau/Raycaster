#include "glfont.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

#include "glprog.h"
#include "filesys.h"

extern ProgramData g_progBasicTex;
extern GLuint g_testVertexArrays;

glFont::glFont() :
	bakedFont("Baked Font")
{
	pixelHeight = 32.0f;
	codePointStart = 32;
	codePointEnd = 96;
	cdata = nullptr;
}

glFont::~glFont()
{
	if (cdata != nullptr)
		delete[] cdata;
}

void glFont::Init(const char* filename)
{
	cdata = nullptr;

	// Setup Image for Baking
	bakedFont.height = 512;
	bakedFont.width = 512;
	bakedFont.imgType = GL_ALPHA;
	bakedFont.imgData = new unsigned char[bakedFont.height*bakedFont.width];

	// Read File into Buffer
	auto ttf_buffer = ReadFileToBuffer(filename);

	// Bake Font to Image
	cdata = new stbtt_bakedchar[codePointEnd - codePointStart];
	stbtt_BakeFontBitmap(&ttf_buffer[0], 0, pixelHeight,
		bakedFont.imgData, bakedFont.width, bakedFont.height, 
		codePointStart, codePointEnd, 
		(stbtt_bakedchar*)cdata); // no guarantee this fits!
	ttf_buffer.clear();

	// Copy to GPU
	bakedFont.CopyToGPU();
	bakedFont.FreeImage();
}

GLuint glFont::GenBindVertList(float x, float y, const std::vector<char>& text)
{
	stbtt_bakedchar* cdata_typed = (stbtt_bakedchar*)cdata;
	
	std::vector<stbtt_aligned_quad> charQuads;
	for (const char& c : text) {
		if ((c < codePointStart) || (c >= codePointEnd)) {
			x += pixelHeight*0.5f;
			continue;
		}

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(cdata_typed, bakedFont.width, bakedFont.height,
			c, &x, &y, &q,
			1); // 1=opengl

		charQuads.push_back(q);
	}

	int i = 0;
	GLuint numFloats = (GLuint)(charQuads.size() * 8);
	GLfloat* vData = new GLfloat[numFloats];

	for (const stbtt_aligned_quad& q : charQuads) {
		vData[i++] = q.s0;
		vData[i++] = q.t1;
		vData[i++] = q.x0;
		vData[i++] = q.y0;

		vData[i++] = q.s1;
		vData[i++] = q.t1;
		vData[i++] = q.x1;
		vData[i++] = q.y0;

		vData[i++] = q.s1;
		vData[i++] = q.t0;
		vData[i++] = q.x1;
		vData[i++] = q.y1;

		vData[i++] = q.s0;
		vData[i++] = q.t0;
		vData[i++] = q.x0;
		vData[i++] = q.y1;
	}

	GLuint newVertArray;

	glGenVertexArrays(1, &newVertArray);
	glBindVertexArray(newVertArray);

	ProgramData* prog = ProgramData::GetProg(PD_BASIC_TEX);
	genBasicFloatNBuffer(0, prog->attribList[0], vData, numFloats, 4);

	return 0; // Can't recall what I was doing here blame the meds
}