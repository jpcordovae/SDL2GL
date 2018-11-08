#include "stdafx.h"
#include "Shader.h"

Shader::Shader(std::string _vertexFileName, std::string _fragmentFileName)
{
	this->ready = false;
	this->vertexFileName = _vertexFileName;
	this->fragmentFileName = _fragmentFileName;
#ifdef _DEBUG
	std::cout << "loading shader... " << _vertexFileName << ", " << _fragmentFileName << std::endl;
#endif
	if (this->Load() < 1) {
		std::cout << "Error loading shader sources" << std::endl;
		return;
	}
#ifdef _DEBUG
	std::cout << "compiling shaders... " << _vertexFileName << ", " << _fragmentFileName << std::endl;
#endif

	if (this->Compile() < 1) {
		std::cout << "Error compiling shader sources" << std::endl;
		return;
	} 
#ifdef _DEBUG
	std::cout << "linking shaders... " << _vertexFileName << ", " << _fragmentFileName << std::endl;
#endif
	if (this->Link() < 1) {
		std::cout << "Error linking shader sources" << std::endl;
		return;
	}
	this->ready = true;
}

bool Shader::Use()
{
	if (this->ready)
	{
		GLCall(glUseProgram(this->shaderProgram));
	}
	else
	{
		return false;
	}
	return true;
}

GLint Shader::GetAttribute(const std::string &name)
{
	return glGetAttribLocation(this->shaderProgram,name.c_str());
}

int Shader::Load()
{
	this->vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try{
		this->vertFile.open(this->vertexFileName,std::ios::in);
	}
	catch (std::system_error &e)
	{
		std::cerr << e.code().message() << std::endl;
		return -1;
	}
	this->vertexSourceStream << vertFile.rdbuf();
	
	this->fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		this->fragFile.open(this->fragmentFileName, std::ios::in);
	}
	catch(std::system_error &e)
	{
		std::cerr << e.code().message() << std::endl;
		return -2;
	}
	this->fragmentSourceStream << fragFile.rdbuf();

	this->vertexSource = this->vertexSourceStream.str();
	this->fragmentSource = this->fragmentSourceStream.str();

	return 1;
}

int Shader::Compile()
{
	const char * tempSource = this->vertexSource.c_str();
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShader, 1, &tempSource, 0);
	glCompileShader(this->vertexShader);
	GLint compiled=0;  
	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		unsigned int maxLength;
		glGetShaderiv(this->vertexShader,GL_INFO_LOG_LENGTH,(GLint*)&maxLength);
		char *vertexCompileLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(this->vertexShader, maxLength, (GLsizei*)&maxLength, vertexCompileLog);
		std::cout << "Vertex Shader error: " << std::endl << vertexCompileLog<< std::endl;
		free(vertexCompileLog);
		return -1;
	}

	tempSource = this->fragmentSource.c_str();
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragmentShader, 1, &tempSource, 0);
	glCompileShader(this->fragmentShader);
	compiled = 0;
	glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		unsigned int maxLength;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, (GLint*)&maxLength);
		char *fragmentCompileLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(this->fragmentShader, maxLength, (GLsizei*)&maxLength, fragmentCompileLog);
		std::cout << "Fragment Shader error: " << std::endl << fragmentCompileLog << std::endl;
		free(fragmentCompileLog);
		return -2;
	}
	return 1;
}

int Shader::Link()
{
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, this->vertexShader);
	glAttachShader(this->shaderProgram, this->fragmentShader);
	glLinkProgram(this->shaderProgram);
	GLint linked=0;
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE)
	{
		unsigned int maxLength;
		glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, (GLint*)&maxLength);
		char *programLinkLog = (char*)malloc(maxLength);
		glGetProgramInfoLog(this->shaderProgram,maxLength,(GLsizei*)&maxLength,programLinkLog);
		std::cout << "Program link error: " << std::endl << programLinkLog << std::endl << std::endl;
		free(programLinkLog);
		return -1;
	}
	return 1;
}

Shader::~Shader()
{
}
