// stdafx.cpp : source file that includes just the standard includes
// SDL2GL.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <strstream>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
void glCheckError(const char* file, unsigned int line, const char* method, const char * op)
{
	// Get the last error
	GLenum lastError = glGetError();

	if (lastError != GL_NO_ERROR) {
		std::string error = "UNDEFINED";

		switch (lastError)
		{
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM";
			break;

		case GL_INVALID_VALUE:
			error = "GL_INVALID_VALUE";
			break;

		case GL_INVALID_OPERATION:
			error = "GL_INVALID_OPERATION";
			break;

		case GL_STACK_OVERFLOW:
			error = "GL_STACK_OVERFLOW";
			break;

		case GL_STACK_UNDERFLOW:
			error = "GL_STACK_UNDERFLOW";
			break;

		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		//std::stringstream  err;
		std::cerr << error << " from " << op << " in " << file << ":" << line << " (inside " << method << ")" << std::endl;
		// << err.str().c_str());
	}
}