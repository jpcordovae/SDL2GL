#pragma once
#include <SDL.h>
#include <iostream>
#include <exception>
#include <memory>
#include <map>
#include <list>
#include "Matrices.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "Scene.h"
#include <SDL.h>
#include "Material.h"

#define SDL_WINDOWS_FLAGS	SDL_WINDOW_OPENGL		| \
							SDL_WINDOW_SHOWN		| \
							SDL_WINDOW_RESIZABLE \


class window_exception : public std::runtime_error
{
public:
	window_exception(const std::string &error) : std::runtime_error(error) {};
};

class gl_context_exception : public std::runtime_error
{
public:
	gl_context_exception(const std::string &error) : std::runtime_error(error) {};
};

class Window
{
public:
	typedef std::shared_ptr<Window> windowPtr;
	Window();
	Window(const std::string &title, int x, int y);
	Window(const std::string &title, int x, int y, int width, int height);
	~Window();
	void Update(SDL_Event e,float dTime);
	void EventHandler(SDL_Event e);
	virtual void Draw(float _dtime);
	virtual void Init();
	GLuint GetSDLWindowID()	{	return sdlWindowID; }
	SDL_GLContext GetSDLContext() {	return sdlGLContext; }
	SDL_Window *GetSDLWindow() {	return sdlWindow; }
protected:
	cameraPtr sdlCamera;
	bool IsInitialized() {
		return initialized;
	}
private: 
	bool initialized = false;
	void InitRenderer();
	SDL_Window * sdlWindow;
	SDL_GLContext sdlGLContext;
	Uint32 sdlWindowID;
	shaderPtr winShader;
	scenePtr scene;
	int wWidth, wHeight; // windows size, resizable
	Matrices pipeline;
	// global variables - normally would avoid globals, using in this demo
	GLuint shaderprogram; // handle for shader program
	GLuint vao, vbo[2]; // handles for our VAO and two VBOs
	float r = 0;
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mWinFocus;
};

typedef Window::windowPtr windowPtr;

extern std::list<windowPtr> windowsContainer;
