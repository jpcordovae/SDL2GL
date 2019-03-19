#include "stdafx.h"
#include "Shader.h"

std::map<std::string, shaderPtr> shader_db;

Shader::Shader(std::string _vertexFileName, std::string _fragmentFileName)
{
	this->ready = false;
	this->vertexFileName = _vertexFileName;
	this->fragmentFileName = _fragmentFileName;
#ifdef _DEBUG
	std::cout << "loading shader files : " << _vertexFileName << ", " << _fragmentFileName << std::endl;
#endif
	if (this->Load() < 1) {
		std::cout << "Error loading shader sources" << std::endl;
		return;
	}
#ifdef _DEBUG
	std::cout << "compiling shader     : " << _vertexFileName << ", " << _fragmentFileName << std::endl;
#endif

	if (this->Compile() < 1) {
		std::cout << "Error compiling shader sources" << std::endl;
		return;
	} 
#ifdef _DEBUG
	std::cout << "linking shader       : " << _vertexFileName << ", " << _fragmentFileName << std::endl;
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
	// vertex
	char * tempSource = (char*) malloc(vertexSource.length()+1);
	memcpy(tempSource,this->vertexSource.c_str(), vertexSource.length());
	tempSource[vertexSource.length()] = 0x00;
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(this->vertexShader, 1, &tempSource, nullptr));
	delete tempSource;
	GLCall(glCompileShader(this->vertexShader));
	GLint compiled=0;  
	GLCall(glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &compiled));
	if (!compiled) {
		unsigned int maxLength;
		GLCall(glGetShaderiv(this->vertexShader,GL_INFO_LOG_LENGTH,(GLint*)&maxLength));
		char *vertexCompileLog = (char *)malloc(maxLength);
		GLCall(glGetShaderInfoLog(this->vertexShader, maxLength, (GLsizei*)&maxLength, vertexCompileLog));
		std::cout << "Vertex Shader error: " << std::endl << vertexCompileLog<< std::endl;
		free(vertexCompileLog);
		delete tempSource;
		return -1;
	}
	// fragment
	tempSource = (char*)malloc(fragmentSource.length() + 1);
	memcpy(tempSource, this->fragmentSource.c_str(), fragmentSource.length());
	tempSource[fragmentSource.length()] = 0x00;
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(this->fragmentShader, 1, &tempSource, nullptr));
	GLCall(glCompileShader(this->fragmentShader));
	compiled = 0;
	GLCall(glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &compiled));
	if (!compiled) {
		unsigned int maxLength;
		GLCall(glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, (GLint*)&maxLength));
		char *fragmentCompileLog = (char *)malloc(maxLength);
		GLCall(glGetShaderInfoLog(this->fragmentShader, maxLength, (GLsizei*)&maxLength, fragmentCompileLog));
		std::cout << "Fragment Shader error: " << std::endl << fragmentCompileLog << std::endl;
		free(fragmentCompileLog);
		delete tempSource;
		return -2;
	}
	delete tempSource;
	return 1;
}

int Shader::Link()
{
	this->shaderProgram = glCreateProgram();
	GLCall(glAttachShader(this->shaderProgram, this->vertexShader));
	GLCall(glAttachShader(this->shaderProgram, this->fragmentShader));
	GLCall(glLinkProgram(this->shaderProgram));
	GLint linked=0;
	GLCall(glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linked));
	if (linked != GL_TRUE)
	{
		unsigned int maxLength;
		GLCall(glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, (GLint*)&maxLength));
		char *programLinkLog = (char*)malloc(maxLength);
		GLCall(glGetProgramInfoLog(this->shaderProgram,maxLength,(GLsizei*)&maxLength,programLinkLog));
		std::cout << "Program link error: " << std::endl << programLinkLog << std::endl << std::endl;
		free(programLinkLog);
		return -1;
	}
	return 1;
}

Shader::~Shader()
{
}
