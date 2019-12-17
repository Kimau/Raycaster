#include "glprog.h"

#include "libs/sdl/include/SDL.h"


void ProgramData::create()
{
	progID = glCreateProgram();
}

void ProgramData::destroy()
{
	for(int i=0; i<shaderList.size(); i++)
	{
		GLuint shaderID = shaderList[i];
		glDetachShader(progID, shaderID);
		glDeleteProgram(shaderID);
	}
	shaderList.clear();

	if (!isValid) 
		return;
	
	glDeleteProgram(progID);
	isValid = false; 
	progID = -1;
}

bool ProgramData::compileAttachShader(const GLchar* source, GLenum type)
{
	// Compile & Attach vertex shader to program
	GLuint shader = compileShader(source, type);
	if (shader == NULL)
		return false;

	return attachShader(shader);
}

bool ProgramData::attachShader(GLuint shader)
{
	glAttachShader(progID, shader);
	shaderList.push_back(shader);
	return true;
}

bool ProgramData::link()
{
	GLint programSuccess = false;

	//Link program
	glLinkProgram(progID);
	glGetProgramiv(progID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", progID);
		printProgramLog(progID);
		return false;
	}

	for (int i = 0; i < uniformList.size(); ++i) {
		if (uniformList[i].PostLink(progID) == false) {
			printf("Invalid Uniform: %s", uniformList[i].name);
			return false;
		}
	}
	for (int i = 0; i < attribList.size(); ++i) {
		if(attribList[i].PostLink(progID) == false) {
		printf("Invalid Attrib: %s", attribList[i].name);
		return false;
		}
	}

	int e = glGetError();
	if (e != GL_NO_ERROR) {
		printf("Program Link Error: %04X", e);
		return false;
	}

	isValid = true;
	return true;
}

bool ProgramData::bind()
{
	if (!isValid)
		return false;

	glUseProgram(progID);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding program! %x\n", error);
		printProgramLog(progID);
		return false;
	}

	return true;
}

ProgramData g_progArray[10];
ProgramData* ProgramData::GetProg(int ID)
{
	return &(g_progArray[ID]);
}

void ProgramData::setupUniform(const GLchar* name)
{
	uniformList.push_back(ShaderUniform(name));
}

void ProgramData::setupAttrib(const GLchar* name)
{
	attribList.push_back(ShaderAttrib(name));
}

void unbindProgram()
{
	glUseProgram(NULL);
}

GLuint compileShader(const GLchar* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL); // One Shader Source which is NULL terminated
	glCompileShader(shader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile shader %d!\n", shader);
		printShaderLog(shader);
		return NULL;
	}

	return shader;
}

void printProgramLog(GLuint program)
{
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

//////////////////////////////////////////////////////////////////////////
// Shader Uniform
ShaderUniform::ShaderUniform(const GLchar* _name) : name(_name), loc(-1)
{
}

bool ShaderUniform::PostLink(GLuint progID) {
	loc = glGetUniformLocation(progID, name);
	return (loc >= 0);
}

void ShaderUniform::Set(int x, int y, int z, int w)
{
	glUniform4i(loc, x, y, z, w);
}

void ShaderUniform::Set(int x, int y, int z)
{
	glUniform3i(loc, x, y, z);
}

void ShaderUniform::Set(int x, int y)
{
	glUniform2i(loc, x, y);
}

void ShaderUniform::Set(int x)
{
	glUniform1i(loc, x);
}

void ShaderUniform::SetF(float x, float y, float z, float w)
{
	glUniform4f(loc, x, y, z, w);
}

void ShaderUniform::SetF(float x, float y, float z)
{
	glUniform3f(loc, x, y, z);
}

void ShaderUniform::SetF(float x, float y)
{
	glUniform2f(loc, x, y);
}

void ShaderUniform::SetF(float x)
{
	glUniform1f(loc, x);
}

void ShaderUniform::SetMatrix2(const float* val, int count /*= 1*/, bool transpose /*= false*/)
{
	glUniformMatrix2fv(loc, count, transpose, val);
}

void ShaderUniform::SetMatrix3(const float* val, int count /*= 1*/, bool transpose /*= false*/)
{
	glUniformMatrix3fv(loc, count, transpose, val);
}

void ShaderUniform::SetMatrix4(const float* val, int count /*= 1*/, bool transpose /*= false*/)
{
	glUniformMatrix4fv(loc, count, transpose, val);
}

//////////////////////////////////////////////////////////////////////////
// Shader Attrib
ShaderAttrib::ShaderAttrib(const GLchar* _name) : name(_name), loc(-1)
{
}

bool ShaderAttrib::PostLink(GLuint progID)
{
	loc = glGetAttribLocation(progID, name);
	return (loc >= 0);
}

void ShaderAttrib::EnableArray()
{
	glEnableVertexAttribArray(loc);
}

void ShaderAttrib::DisableArray()
{
	glDisableVertexAttribArray(loc);
}

//////////////////////////////////////////////////////////////////////////

void genBasicFloatNBuffer(GLuint index, ShaderAttrib& attrib, GLfloat* vertData, GLuint length, GLuint numFloatrs)
{
	GLuint tempVertBufferToCopy;

	glGenBuffers(1, &tempVertBufferToCopy);
	glBindBuffer(GL_ARRAY_BUFFER, tempVertBufferToCopy);

	attrib.EnableArray();
	glVertexAttribPointer(index, numFloatrs, GL_FLOAT, false, 0, 0);

	glBufferData(GL_ARRAY_BUFFER, length * numFloatrs * sizeof(GLfloat), vertData, GL_STATIC_DRAW); // Static/Stream/Dynamic

	attrib.DisableArray();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


