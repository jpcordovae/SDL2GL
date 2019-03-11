#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <memory>
#include "Utils.h"
#include <exception>
#include <map>

class gl_shader_exception : public std::runtime_error
{
public:
	gl_shader_exception(const std::string &error) :std::runtime_error(error) {};
};

class Shader
{
public:
	typedef std::shared_ptr<Shader> shaderPtr;

	Shader(std::string vertexFileName, std::string fragmentFileName);
	bool Use();
	GLint GetAttribute(const std::string &name);
	GLuint GetShaderProgram() { return shaderProgram; }
	virtual ~Shader();
private:
	std::ifstream vertFile, fragFile;
	std::string vertexFileName, fragmentFileName;
	std::stringstream vertexSourceStream, fragmentSourceStream;
	std::string vertexSource, fragmentSource;
	GLuint vertexShader, fragmentShader, shaderProgram;
	bool ready;
	int Load();
	int Compile();
	int Link();
};

typedef Shader::shaderPtr shaderPtr;
extern std::map<std::string, shaderPtr> shader_db;
