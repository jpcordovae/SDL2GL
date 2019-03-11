#pragma once

#define GL3_PROTOTYPES 1
#include "Window.h"
#include "TetahedraWindows.h"

class application_exception : public std::runtime_error
{
public:
	application_exception(const std::string &error) :std::runtime_error(error) {};
};

class Application
{
	Application();
	~Application();
	static Application* applicationInstance;
public:
	static Application* Instance();
	bool Run();
	void Stop();
private:
	bool active;
	windowPtr window;
	tetahedraWindowPtr TWindow;
};
