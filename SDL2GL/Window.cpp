#include "stdafx.h"
#include "Window.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include "Utils.h"
#include <filesystem>
#include "temporal.h"

std::list<windowPtr> windowsContainer;

Window::Window(const std::string &title, int x, int y, int width, int height)
{
	this->sdlWindow = SDL_CreateWindow(title.c_str(), 
										x, y, width, height, 
										SDL_WINDOWS_FLAGS);
	if (this->sdlWindow == nullptr)
	{
		throw window_exception(SDL_GetError());
	}
	
	this->wWidth = width;
	this->wHeight = height;
	sdlCamera = cameraPtr(new Camera());
	this->InitRenderer();
	sdlWindowID = SDL_GetWindowID(this->sdlWindow);
	initialized = true;
	windowsContainer.emplace_back(std::shared_ptr<Window>(this)); // add windows to the windows container managed by the aplication.
}

Window::Window(const std::string &title, int width, int height)
{
	this->sdlWindow = SDL_CreateWindow(title.c_str(), 
										SDL_WINDOWPOS_CENTERED, 
										SDL_WINDOWPOS_CENTERED, 
										width, height, SDL_WINDOWS_FLAGS);

	if (this->sdlWindow == nullptr)
	{
		throw window_exception(SDL_GetError());
	}

	this->wWidth = width;
	this->wHeight = height;
	sdlCamera = cameraPtr(new Camera(glm::vec3(0.0f, 0.0f, 0.0f)));//DEFAULT CAMERA POSITION
	sdlWindowID = SDL_GetWindowID(this->sdlWindow); 
	this->InitRenderer();
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		exit(1);
	}
	//LoadShaders();
	this->Init();
	windowsContainer.emplace_back(std::shared_ptr<Window>(this)); // add windows to the windows container managed by the aplication.
	initialized = true;
}

void Window::Init()
{
	SDL_GL_MakeCurrent(this->sdlWindow, this->sdlGLContext);

	if ((this->winShader = shaderPtr(new Shader("Shaders/vertex.vert", "Shaders/fragment.frag"))) == nullptr)
	{
		throw gl_shader_exception("error creating shader!!!");
	}

	std::string scene_file = "3DModels/cube.obj";
	if (!std::experimental::filesystem::exists(scene_file.c_str()))
	{
		throw window_exception("scene file not found (experimental)");
	}

	this->scene = scenePtr(new Scene(scene_file.c_str()));
	mMouseFocus = false;
	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//GLCall(glEnable(GL_BLEND));
	//GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA)); 
}

void Window::InitRenderer()
{
	this->sdlGLContext = SDL_GL_CreateContext(this->sdlWindow) ;
	if (this->sdlGLContext == nullptr)
	{
		throw gl_context_exception(SDL_GetError());
	}
	if ( SDL_GL_SetSwapInterval(1) < 0)
	{
		throw gl_context_exception(SDL_GetError());
	}
}

void Window::Draw(float _dTime)
{
	if (!this->initialized || !((bool)sdlCamera)) return;
	SDL_GL_MakeCurrent(this->sdlWindow, this->sdlGLContext);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	
	static float rotAngle = 0.0f;
	glm::mat4 model = glm::rotate(rotAngle+=0.01f,glm::vec3(0.0f,1.0f,0.0f));
	sdlCamera->setLocation(glm::vec3(0.0f, 0.0f, 2.0f));
	glm::mat4 view = glm::lookAt(sdlCamera->getLocation(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * wWidth / wHeight, 0.1f, 100.0f);
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProjectionMatrix = projection * view * model;
	glm::mat3 normalMatrix = glm::mat3(modelView);

	this->winShader->Use();
	
	int programId = this->winShader->GetShaderProgram();

	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix)));

	GLCall(glUniform3f(glGetUniformLocation(programId, "cameraPosition"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "lightPos"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	this->scene->Draw(this->winShader->GetShaderProgram());

	SDL_GL_SwapWindow(sdlWindow); // swap buffers
}

void Window::EventHandler(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_WINDOWEVENT:
		switch (e.window.event)
		{
			//Window appeared
		case SDL_WINDOWEVENT_SHOWN:
			//mShown = true;
			break;
			//Window disappeared
		case SDL_WINDOWEVENT_HIDDEN:
			//mShown = false;
			break;
			//Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			//mWidth = e.window.data1;
			//mHeight = e.window.data2;
			//SDL_RenderPresent(mRenderer);
			glViewport(0, 0, e.window.data1, e.window.data2);
			wWidth = e.window.data1;
			wHeight = e.window.data2;
			break;
			//Repaint on expose
		case SDL_WINDOWEVENT_EXPOSED:
			//SDL_RenderPresent(mRenderer);
			break;
			//Mouse enter
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			sdlCamera->mouseIn(true);
			std::cout << "mouse in" << std::endl;
			//updateCaption = true;
			break;
			//Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			if (mMouseFocus)
			{
				std::cout << "mouse out" << std::endl;
				mMouseFocus = false;
				sdlCamera->mouseIn(false);
				//updateCaption = true;
			}
			break;
			//Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			std::cout << "win focus gained" << std::endl;
			mWinFocus = true;
			//updateCaption = true;
			break;
			//Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			mWinFocus = false;
			std::cout << "win focus lost" << std::endl;
			//updateCaption = true;
			break;
			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			//mMinimized = true;
			break;
			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			//mMinimized = false;
			break;
			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			//mMinimized = false;
			break;
			//Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			//SDL_HideWindow(mWindow);
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_RIGHT:
			if (mMouseFocus)
			{
				sdlCamera->SetIsMoved(true);
				std::cout << "right mouse button down." << std::endl;
				SDL_ShowCursor(SDL_DISABLE);
			}
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (e.button.button)
		{
		case SDL_BUTTON_RIGHT:
			if (mMouseFocus)
			{
				sdlCamera->SetIsMoved(false);
				std::cout << "right mouse button up." << std::endl;
				SDL_ShowCursor(SDL_ENABLE);
			}
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		if (sdlCamera->isMoved()) 
		{
			int dx = e.motion.xrel;
			int dy = e.motion.yrel;
			
#ifdef _DEBUG
			std::cout << "mouse moved dx=" << dx << ",dy=" << dy << std::endl;
#endif
			int MidX, MidY;
			SDL_GetWindowSize(sdlWindow, &MidX, &MidY);
			MidX = (int)MidX / 2;
			MidY = (int)MidY / 2;
			//SDL_WarpMouseInWindow(sdlWindow, MidX, MidY);
		}
		break;
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			//this->Stop();
			break;
		case SDLK_w:
			//this->ismoved = true;
			/*if (camPitch != 90 && camPitch != -90)
				moveCamera(0.0);
			moveCameraUp(0.0);*/
			//this->setLocation(glm::vec3(loc.x + 1, loc.y, loc.z));
			std::cout << "SDLK_w : " << std::endl;
			break;
		case SDLK_s:
			//this->ismoved = true;
			/*if (camPitch != 90 && camPitch != -90)
				moveCamera(180.0);
			moveCameraUp(180.0);*/
			//this->setLocation(glm::vec3(loc.x - 1, loc.y, loc.z));
			std::cout << "SDLK_s : " << std::endl;
			break;
		case SDLK_a:
			/*this->ismoved = true;
			moveCamera(90.0);*/
			//this->setLocation(glm::vec3(loc.x, loc.y, loc.z - 1));
			std::cout << "SDLK_a : " << std::endl;
			break;
		case SDLK_d:
			/*this->ismoved = true;
			moveCamera(270);*/
			//this->setLocation(glm::vec3(loc.x, loc.y, loc.z + 1));
			std::cout << "SDLK_d : " << std::endl;
			break;
		default:
			break;
		}
		//viewMatrix = glm::lookAt(loc, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case SDL_KEYUP:
		//this->ismoved = false;
		//SDL_ShowCursor(SDL_ENABLE);
		break;
	default:
		break;
	}
}

Window::~Window()
{
	SDL_GL_DeleteContext(this->sdlGLContext);
	SDL_DestroyWindow(this->sdlWindow);
}


