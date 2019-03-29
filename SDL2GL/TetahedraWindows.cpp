#include "TetahedraWindows.h"
#include "glm/gtx/normal.hpp"
#include <array>
#include <map>
#include <algorithm>
#include <chrono>

TetahedraWindow::TetahedraWindow(int argc, char **argv)
{
	simulation_running = false;
	if (!b.parse_commandline(argc,argv)) {
		terminatetetgen(NULL, 10);
	}

	// Read input files.
	if (b.refine) { // -r
		if (!in.load_tetmesh(b.infilename, (int)b.object)) {
			terminatetetgen(NULL, 10);
		}
	}else 
	{ // -p
		if (!in.load_plc(b.infilename, (int)b.object)) {
			terminatetetgen(NULL, 10);
		}
	}

	if (b.insertaddpoints) { // -i 
	  // Try to read a .a.node file.
		addin.load_node(b.addinfilename);
	}

	if (b.metric) { // -m 
	  // Try to read a background mesh in files .b.node, .b.ele.
		bgmin.load_tetmesh(b.bgmeshfilename, (int)b.object);
	}

	tetrahedralize(&b, &in, &out, &addin, &bgmin);
	
	if ((this->tetShader = shaderPtr(new Shader("Shaders/mesh_shader.vert", "Shaders/mesh_shader.frag"))) == nullptr)
	{
		throw gl_shader_exception("error creating shader!!!");
	}

	if ((this->tetSurfShader = shaderPtr(new Shader("Shaders/mesh_surface.vert", "Shaders/mesh_surface.frag"))) == nullptr)
	{
		throw gl_shader_exception("error creating shader!!!");
	}

	std::vector<stVertexData> vertex_db;
	stVertexData vd;

	for (int i = 0; i < out.numberofpoints; i++)
	{
		vd.position = glm::vec3(out.pointlist[i * 3 + 0] - out.firstnumber, 
								out.pointlist[i * 3 + 1] - out.firstnumber, 
								out.pointlist[i * 3 + 2] - out.firstnumber);
		vertex_db.push_back(vd);
	}
	
	std::vector<unsigned int> faceindex;
	std::vector<glm::vec3> face_normals;
	glm::vec3 normTmp;

	for (size_t i = 0; i < out.numberoftrifaces; i++)
	{
		faceindex.push_back((unsigned int)out.trifacelist[i * 3 + 0] - out.firstnumber);
		faceindex.push_back((unsigned int)out.trifacelist[i * 3 + 1] - out.firstnumber);
		faceindex.push_back((unsigned int)out.trifacelist[i * 3 + 2] - out.firstnumber);

		normTmp = glm::triangleNormal(vertex_db[out.trifacelist[i * 3 + 0] - out.firstnumber].position,
									  vertex_db[out.trifacelist[i * 3 + 1] - out.firstnumber].position,
									  vertex_db[out.trifacelist[i * 3 + 2] - out.firstnumber].position);

		face_normals.push_back(normTmp);

		vertex_db[out.trifacelist[i * 3 + 0] - out.firstnumber].normal += normTmp;
		vertex_db[out.trifacelist[i * 3 + 1] - out.firstnumber].normal += normTmp;
		vertex_db[out.trifacelist[i * 3 + 2] - out.firstnumber].normal += normTmp;
	}

	for (int i = 0; i < out.numberofpoints; i++)
	{
		vertex_db[i].normal = glm::normalize(vertex_db[i].normal);
	}

	std::vector<unsigned int> trindex;
	for (size_t i = 0; i < out.numberoftetrahedra; i++)
	{
		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);

		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);

		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2] - out.firstnumber);

		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3] - out.firstnumber);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0] - out.firstnumber);

	}

	tetamesh->SetMeshData(&vertex_db, &trindex);
	tetamesh->SetDisplayType(Mesh::RAW_MESH);
	
	//TODO: automate start camera position given boundaries of the model
	sdlCamera->Location(glm::vec3(0.0f, 0.0f, 300.0f));

	surfMesh->SetMeshData(&vertex_db, &faceindex);
	surfMesh->SetDisplayType(Mesh::RAW_TRIANGLES);

	//TODO: fix depth and colors
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_FRONT));

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//TODO: fix this swap, must be a better way
	std::thread Sim(&TetahedraWindow::UpdateSimulation,this);
	simulation_thread.swap(Sim);
}


void TetahedraWindow::InitSimulationData()
{
	vertex_db_0.resize(vertex_db.size());
	vertex_db_1.resize(vertex_db.size());
	vertex_db_0 = vertex_db;

	std::vector<arma::mat33> X;
	X.resize(out.numberoftetrahedra);
	arma::mat33 Xi[4];
	arma::vec3 Xi0;
	arma::mat33 M;

	for (size_t i = 0; i < out.numberoftetrahedra; i++)
	{
		for (size_t j = 1; j < 4; j++)
		{
			double i0 = out.pointlist[out.tetrahedronlist[i * 4 + j] + 0 - out.firstnumber] - out.pointlist[out.tetrahedronlist[i * 4 + 0] + 0 - out.firstnumber];
			double i1 = out.pointlist[out.tetrahedronlist[i * 4 + j] + 1 - out.firstnumber] - out.pointlist[out.tetrahedronlist[i * 4 + 0] + 1 - out.firstnumber];
			double i2 = out.pointlist[out.tetrahedronlist[i * 4 + j] + 2 - out.firstnumber] - out.pointlist[out.tetrahedronlist[i * 4 + 0] + 2 - out.firstnumber];

			Xi0 << i0 << i1 << i2;
			Xi[j - 1].col(j - 1) = Xi0;
		}


	}

	vertex_db_0 = vertex_db;
}

void TetahedraWindow::Draw(float _dtime)
{
	if (!this->IsInitialized()) return;
	SDL_GL_MakeCurrent(GetSDLWindow(),GetSDLContext());
	// glewInit must be called after each context change
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		exit(1);
	}
	//
	glViewport(0, 0, wWidth, wHeight);
	glClearColor(0.0f,0.0f,0.0f, 1.0f);
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//
	float rotAngle = 0.0f;
	//glm::mat4 model = glm::rotate(rotAngle += 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(sdlCamera->getVector(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * GetWindowWidth() / GetWindowHeight(), 0.1f, 100000.0f);
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProjectionMatrix = projection * view * model;
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelView));
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, -1.0f);

	int programId=0;

	glDepthMask(1);
	tetShader->Use();
	// copy asynchronic data
	simulation_mutex.lock();
	//glBufferSubData(0, 0, vertex_db_0.size(), vertex_db_0.data());
	simulation_mutex.unlock();
	programId = tetShader->GetShaderProgram();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix)));
	GLCall(glUniform3f(glGetUniformLocation(programId, "cameraPosition"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "lightPos"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "ambientColor"), 1.0f, 1.0f, 1.0f));
	tetamesh->draw(programId);
	
	glDepthMask(1);
	tetSurfShader->Use();
	programId = tetSurfShader->GetShaderProgram();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix)));
	GLCall(glUniform3f(glGetUniformLocation(programId, "cameraPosition"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "lightPos"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "ambientColor"), 0.0f, 0.0f, 1.0f));
	surfMesh->draw(programId);

	SDL_GL_SwapWindow(GetSDLWindow()); // swap buffers
}

void TetahedraWindow::UpdateSimulation()
{
	bool running;
	simulation_mutex.lock();
	simulation_running = true;
	running = simulation_running;
	simulation_mutex.unlock();
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
	double dt = 0;

	while (running) 
	{
		now = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::microseconds>(now - t0).count() / 1000000.0f;
		//update simulation
		for (size_t i = 0; i < out.numberoftetrahedra; i++)
		{
			// 
		}
		//data copy

		//update VAO
		simulation_mutex.lock();
		running = simulation_running;
		simulation_mutex.unlock();

		vertex_db_0.clear();
		std::copy(vertex_db_1.begin(), vertex_db_1.end(), std::back_inserter(vertex_db_0));
	}
}

TetahedraWindow::~TetahedraWindow()
{
}


