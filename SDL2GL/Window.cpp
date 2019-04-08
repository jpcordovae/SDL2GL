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

Window::Window():Window("void title",100, 100, 800, 600)
{
	
}

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
	
	sdlWindowID = SDL_GetWindowID(this->sdlWindow);

	this->sdlGLContext = SDL_GL_CreateContext(this->sdlWindow);

	if (this->sdlGLContext == nullptr)
	{
		throw gl_context_exception(SDL_GetError());
	}
	
	SDL_GL_MakeCurrent(this->sdlWindow, this->sdlGLContext);

	// glewInit must be called after each context change
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		exit(1);
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		throw gl_context_exception(SDL_GetError());
	}
	
	sdlCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 8.0f));

	initialized = true;

	Initialization();

	windowsContainer.emplace_back(this); // add windows to the windows container managed by the aplication.
}

Window::Window(const std::string &title, int width, int height) 
	: Window(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width,height)
{

}

void Window::Draw(float _dTime)
{
	if (!this->IsInitialized()) return;
	SDL_Window * winTmp = this->GetSDLWindow();
	SDL_GLContext winContext = this->GetSDLContext();
	SDL_GL_MakeCurrent(winTmp, winContext);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	/*
	static float rotAngle = 0.0f;
	//glm::mat4 model = glm::rotate(rotAngle+=0.01f,glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 model = glm::rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//sdlCamera->setLocation(glm::vec3(0.0f, 0.0f, 8.0f));
	glm::mat4 view = glm::lookAt(sdlCamera->getVector(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 4.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * wWidth / wHeight, 0.1f, 10000.0f);
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
	*/

	SDL_GL_SwapWindow(this->GetSDLWindow()); // swap buffers
}

void Window::EventHandler(SDL_Event e)
{
	if (e.window.windowID != this->GetSDLWindowID()) return;

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
			//std::cout << "mouse in" << std::endl;
			//updateCaption = true;
			break;
			//Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			if (mMouseFocus)
			{
				//std::cout << "mouse out" << std::endl;
				mMouseFocus = false;
				sdlCamera->mouseIn(false);
				//updateCaption = true;
			}
			break;
			//Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			//std::cout << "win focus gained" << std::endl;
			mWinFocus = true;
			//updateCaption = true;
			break;
			//Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			mWinFocus = false;
			sdlCamera->SetIsMoved(false);
			//std::cout << "win focus lost" << std::endl;
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
			sdlCamera->Rotating(true);
			SDL_ShowCursor(SDL_DISABLE);
			break;
		case SDL_BUTTON_MIDDLE:
			sdlCamera->Zooming(true);
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (e.button.button)
		{
		case SDL_BUTTON_RIGHT:
			sdlCamera->Rotating(false);
			SDL_ShowCursor(SDL_ENABLE);
			break;
		case SDL_BUTTON_MIDDLE:
			sdlCamera->Zooming(false);
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		if (sdlCamera->Rotating())
		{
			sdlCamera->setPitch(sdlCamera->getPitch() + (float)e.motion.yrel);
			sdlCamera->setYaw(sdlCamera->getYaw()- (float)e.motion.xrel);
			//SDL_GetWindowSize(sdlWindow, &MidX, &MidY);
			//MidX = (int)MidX / 2;
			//MidY = (int)MidY / 2;
			//SDL_WarpMouseInWindow(sdlWindow, MidX, MidY);
		}
		if (sdlCamera->Zooming())
		{
			sdlCamera->SetDistance(sdlCamera->GetDistance() + (float)e.motion.yrel);
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
			/*#ifdef _DEBUG
			std::cout << "SDLK_w : " << std::endl;
			#endif*/
			break;
		case SDLK_s:
			//this->ismoved = true;
			/*if (camPitch != 90 && camPitch != -90)
				moveCamera(180.0);
			moveCameraUp(180.0);*/
			//this->setLocation(glm::vec3(loc.x - 1, loc.y, loc.z));
			#ifdef _DEBUG
			std::cout << "SDLK_s : " << std::endl;
			#endif
			break;
		case SDLK_a:
			/*this->ismoved = true;
			moveCamera(90.0);*/
			//this->setLocation(glm::vec3(loc.x, loc.y, loc.z - 1));
			#ifdef _DEBUG
			std::cout << "SDLK_a : " << std::endl;
			#endif
			break;
		case SDLK_d:
			/*this->ismoved = true;
			moveCamera(270);*/
			//this->setLocation(glm::vec3(loc.x, loc.y, loc.z + 1));
			#ifdef _DEBUG
			std::cout << "SDLK_d : " << std::endl;
			#endif
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

void Window::Initialization()
{

}

Window::~Window()
{
	SDL_GL_DeleteContext(this->sdlGLContext);
	SDL_DestroyWindow(this->sdlWindow);
}


