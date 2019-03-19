#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL\GLU.h>
#include <iostream>
#include <assimp\types.h>
#include <assimp\color4.h>

bool file_exist(const char *fileName);
std::ostream& operator<<(std::ostream& os, const  glm::vec3 &vector);

#define ASSERTGL(x) if(!(x)) __debugbreak();

#define GLCall(x)	x;\
					ASSERTGL(GLLogCall(#x,__FILE__,__LINE__));\
					GLClearError();

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);
std::string getBasePath(const std::string& path);
void vec4_copy(glm::vec4 &dColor, const aiColor4D &oColor);

