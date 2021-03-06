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
	void EventHandler(SDL_Event e);
	virtual void Draw(float _dtime);
	GLuint GetSDLWindowID()	{	return sdlWindowID; }
	SDL_GLContext GetSDLContext() {	return sdlGLContext; }
	SDL_Window *GetSDLWindow() {	return sdlWindow; }
	unsigned int GetWindowWidth() { return wWidth; }
	unsigned int GetWindowHeight() { return wHeight; }
	bool Initialized() { return initialized; }
	virtual void Initialization();
protected:
	cameraPtr sdlCamera;
	bool IsInitialized() {
		return initialized;
	}
	shaderPtr winShader;
	int wWidth, wHeight; // windows size, resizable
private: 
	bool initialized = false;
	SDL_Window * sdlWindow;
	SDL_GLContext sdlGLContext;
	SDL_Renderer *sdlRenderer;
	Uint32 sdlWindowID;
	
	Matrices pipeline;
	// global variables - normally would avoid globals, using in this demo
	GLuint shaderprogram; // handle for shader program
	bool mMiddButtonPressed;
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mWinFocus;
};

typedef Window::windowPtr windowPtr;

extern std::list<windowPtr> windowsContainer;
