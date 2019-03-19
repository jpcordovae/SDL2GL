#include "stdafx.h"
#include "Application.h"
#include <SDL.h>
#include <Windows.h>
#include <GL/GL.h>
#include <algorithm>
#include "Utils.h"

Application* Application::applicationInstance =nullptr;

Application::Application(int _argc, char *_argv[]) 
{
	set_command_line(_argc,_argv);
	try {
		this->active = true;

		if ( SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			throw application_exception(SDL_GetError());
		}		

		int flags = IMG_INIT_JPG ;
		int initted = IMG_Init(flags);
		if ((initted&flags) != flags) {
			printf("IMG_Init: Failed to init required jpg and png support!\n");
			printf("IMG_Init: %s\n", IMG_GetError());
			// handle error
		}

		//Use OpenGL 4.6 core
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = windowPtr(new Window("hello world", 640,480));
		this->TWindow = tetahedraWindowPtr(new TetahedraWindow(_argc,_argv));
	}
	catch (application_exception& exception)
	{
		std::cout << "Application error : ";
#ifdef _DEBUG
		std::cout << "Line 24 " << " : ";
#endif
		std::cout << exception.what() << std::endl;
	}
	catch (window_exception &exception)
	{
		std::cout << "Window error : ";
#ifdef _DEBUG
		std::cout << "Line 32  : ";
#endif
		std::cout << exception.what() << std::endl;
	}
	catch (gl_context_exception &exception)
	{
		std::cout << "GLContext error : ";
#ifdef _DEBUG
		std::cout << "Line 39  : ";
#endif
		std::cout << exception.what() << std::endl;
	}
	catch (scene_exception &e)
	{
		std::cerr << "scene exeption :";
#ifdef _DEBUG
		std::cout << "line "<< __LINE__ << " : ";
#endif
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	
}

void Application::set_command_line(int _argc, char *_argv[])
{
	size_t index = 0;
	if (argv == nullptr)
	{
		argv = (char **)malloc(_argc);
		for (int i = 0; i < _argc; i++)
		{
			size_t size = strlen(_argv[i]);
			argv[i] = (char *) malloc(size+1);
			ZeroMemory(argv[i], size+1);
			memcpy(argv[i], _argv[i], size+1);
		}
	}
	argc = _argc;
}

Application* Application::Instance()
{
	return Application::applicationInstance;
}

Application* Application::Instance(int _argc, char *_argv[])
{
	if (Application::applicationInstance == nullptr)
	{
		Application::applicationInstance = new Application(_argc,_argv);
	}
	return Application::applicationInstance;
}

bool Application::Run()
{
	static UINT32 old_time = SDL_GetTicks();
	
	while (this->active)
	{
		Uint32 now = SDL_GetTicks();
		float dTime = (now - old_time) / 1000.0f; // ms

		SDL_Event event;
		while (SDL_PollEvent(&event)!=0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				this->Stop();
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					this->Stop();
					break;
				case SDLK_w:
					break;
				case SDLK_s:
					break;
				case SDLK_a:
					break;
				case SDLK_d:
					break;
				default:
					break;
				}
				break;
			case SDL_WINDOWEVENT:
				break;
			case SDL_MOUSEMOTION:
				break; 
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			}
			for (windowPtr wp : windowsContainer)
			{
				wp->EventHandler(event); // update events
			}
				 
		}
		// draw windows
		for (windowPtr wp : windowsContainer)
		{
			wp->Draw(dTime);
		}
		
		old_time = now;
	}
	return this->active;
}

void Application::Stop()
{
	this->active = false;
}

Application::~Application()
{
	IMG_Quit();
	SDL_Quit();
}
