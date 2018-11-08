#include "stdafx.h"
#include "Utils.h"
#include <fstream>

bool file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void vec4_copy(glm::vec4 &dColor, const aiColor4D &oColor)
{
	dColor.r = oColor.r;
	dColor.g = oColor.g;
	dColor.b = oColor.b;
	dColor.a = oColor.a;
}

std::ostream& operator<<(std::ostream& os, const  glm::vec3 &vector)
{
	os << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
	return os;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL error] (" << error << ")" << function << " , " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

std::string getBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}
