#pragma once

#define GL3_PROTOTYPES 1
#include "Window.h"
#include "TetahedraWindows.h"
#include "TexturedWindow.h"

class application_exception : public std::runtime_error
{
public:
	application_exception(const std::string &error) :std::runtime_error(error) {};
};

class Application
{
	Application(int _argc, char *argv[]);
	~Application();
	static Application* applicationInstance;
public:
	static Application* Instance(int _argc, char *_argv[]);
	static Application* Instance();
	void set_command_line(int _argc, char *_argv[]);
	bool Run();
	void Stop();
private:
	int argc;
	char **argv;
	bool active;
	windowPtr window;
	texturedWindowPtr txtwPtr;
	tetahedraWindowPtr TWindow;
};
