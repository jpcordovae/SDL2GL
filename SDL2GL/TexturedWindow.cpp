#include "TexturedWindow.h"
#include "Utils.h"
#include <filesystem>

TexturedWindow::TexturedWindow()
{

}

void TexturedWindow::Draw(float _dTime)
{
	if (!this->IsInitialized() || !((bool)sdlCamera)) return;
	SDL_GL_MakeCurrent(this->GetSDLWindow(), this->GetSDLContext());
	
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	static float rotAngle = 0.0f;
	//glm::mat4 model = glm::rotate(rotAngle+=0.01f,glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 model = glm::rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//sdlCamera->setLocation(glm::vec3(0.0f, 0.0f, 8.0f));
	glm::mat4 view = glm::lookAt(sdlCamera->getVector(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 4.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * GetWindowWidth() / GetWindowHeight(), 0.1f, 10000.0f);
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

	SDL_GL_SwapWindow(this->GetSDLWindow()); // swap buffers
}

void TexturedWindow::Init()
{
	SDL_GL_MakeCurrent(this->GetSDLWindow(), this->GetSDLContext());
	// glewInit must be called after each context change
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(glew_status) << "\n";
		exit(1);
	}
	//
	if ((this->winShader = shaderPtr(new Shader("Shaders/vertex.vert", "Shaders/fragment.frag"))) == nullptr)
	{
		throw gl_shader_exception("error creating shader!!!");
	}
	//
	std::string scene_file = "3DModels/cube.obj";
	
	if (!std::experimental::filesystem::exists(scene_file.c_str()))
	{
		throw window_exception("scene file not found (experimental)");
	}
	//
	this->scene = scenePtr(new Scene(scene_file.c_str()));

	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

TexturedWindow::~TexturedWindow()
{

}

