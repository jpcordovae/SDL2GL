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

	stVertexData vd;
	a_vertex_db_0.resize(out.numberofpoints);
	a_vertices.resize(out.numberofpoints);
	a_forces.resize(out.numberofpoints);
	a_speed.resize(out.numberofpoints);
	f_vertices.resize(out.numberofpoints * 3);
	for (int i = 0; i < out.numberofpoints; i++)
	{
		arma::vec3 av;

		vd.position = glm::vec3(out.pointlist[i * 3 + 0],
			out.pointlist[i * 3 + 1],
			out.pointlist[i * 3 + 2]);

		vertex_db.push_back(vd);

		av << out.pointlist[i * 3 + 0]
			<< out.pointlist[i * 3 + 1]
			<< out.pointlist[i * 3 + 2];

		a_vertex_db_0[i] = av;
		a_vertices[i] = av;
	}
	
	std::vector<unsigned int> faceindex;
	std::vector<glm::vec3> face_normals;
	glm::vec3 normTmp;

	for (size_t i = 0; i < out.numberoftrifaces; i++)
	{
		size_t tmp1 = (unsigned int)out.trifacelist[i * 3 + 0] - out.firstnumber;
		size_t tmp2 = (unsigned int)out.trifacelist[i * 3 + 1] - out.firstnumber;
		size_t tmp3 = (unsigned int)out.trifacelist[i * 3 + 2] - out.firstnumber;

		faceindex.push_back(tmp1);
		faceindex.push_back(tmp2);
		faceindex.push_back(tmp3);

		normTmp = glm::triangleNormal(vertex_db[tmp1].position,
									  vertex_db[tmp2].position,
									  vertex_db[tmp3].position);

		face_normals.push_back(normTmp);

		vertex_db[tmp1].normal += normTmp;
		vertex_db[tmp2].normal += normTmp;
		vertex_db[tmp3].normal += normTmp;

	}

	for (int i = 0; i < out.numberofpoints; i++)
	{
		vertex_db[i].normal = glm::normalize(vertex_db[i].normal);
	}

	std::vector<unsigned int> trindex;

	for (size_t i = 0; i < out.numberoftetrahedra; i++)
	{
		unsigned int p0 = out.tetrahedronlist[i * 4 + 0] - out.firstnumber;
		unsigned int p1 = out.tetrahedronlist[i * 4 + 1] - out.firstnumber;
		unsigned int p2 = out.tetrahedronlist[i * 4 + 2] - out.firstnumber;
		unsigned int p3 = out.tetrahedronlist[i * 4 + 3] - out.firstnumber;

		// face A
		trindex.push_back(p0);
		trindex.push_back(p1);
		trindex.push_back(p1);
		trindex.push_back(p2);
		trindex.push_back(p2);
		trindex.push_back(p0);
		// face B
		trindex.push_back(p1);
		trindex.push_back(p2);
		trindex.push_back(p2);
		trindex.push_back(p3);
		trindex.push_back(p3);
		trindex.push_back(p1);
		// face C
		trindex.push_back(p2);
		trindex.push_back(p3);
		trindex.push_back(p3);
		trindex.push_back(p0);
		trindex.push_back(p0);
		trindex.push_back(p2);
		// face D
		trindex.push_back(p0);
		trindex.push_back(p1);
		trindex.push_back(p1);
		trindex.push_back(p3);
		trindex.push_back(p3);
		trindex.push_back(p0);

		tetahedra tet;

		tet.vertices_index.push_back(p0);
		tet.vertices_index.push_back(p1);
		tet.vertices_index.push_back(p2);
		tet.vertices_index.push_back(p3);
		
		tet.X.col(0) = a_vertex_db_0[p1] - a_vertex_db_0[p0];
		tet.X.col(1) = a_vertex_db_0[p2] - a_vertex_db_0[p0];
		tet.X.col(2) = a_vertex_db_0[p3] - a_vertex_db_0[p0];

		tet.P = tet.X;

		try
		{ 
			tet.X = arma::inv(tet.X);
		}catch(std::exception &e)
		{ 
			std::cerr << e.what() << std::endl;
			std::cout << "P:" << p0 << "," << p1 << "," << p2 << "," << p3 << std::endl;
			tet.X.print("tet.X");
			continue;
		}

		// faces index
		std::vector<unsigned int> findex;
		findex.push_back(p0);
		findex.push_back(p1);
		findex.push_back(p2);
		tet.faces_index.push_back(findex);
		findex.clear();
		findex.push_back(p1);
		findex.push_back(p2);
		findex.push_back(p3);
		tet.faces_index.push_back(findex);
		findex.clear();
		findex.push_back(p0);
		findex.push_back(p2);
		findex.push_back(p3);
		tet.faces_index.push_back(findex);
		findex.clear();
		findex.push_back(p0);
		findex.push_back(p1);
		findex.push_back(p3);
		tet.faces_index.push_back(findex);

		a_tetahedra_db.push_back(tet);
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
	simulation_running = false;
	InitSimulationData();
	std::thread Sim(&TetahedraWindow::UpdateSimulation,this);
	simulation_thread.swap(Sim);
	Initialization();
}

void TetahedraWindow::Initialization()
{
	InitSimulationData();
	simulation_running = true;
}

void TetahedraWindow::InitSimulationData()
{
	/*a_vertex_db_0.resize(a_vertex_db.size());
	a_vertex_db_1.resize(a_vertex_db.size());
	
	a_vertex_db_0 = a_vertex_db;*/
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
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_FRONT)); 
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
	GLCall(glDisable(GL_CULL_FACE));
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
	double dt = 0; // [s]
	double E = 0.5;
	arma::vec3 fg(arma::fill::zeros);
	fg.at(1) = -9.8f; // [m*s^2]
	double m = 0.1f; //[Kg]
	while (!running) {};
	while (running) 
	{
		now = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::microseconds>(now - t0).count() / 1000000.0f;
		//update simulation
		for (size_t i = 0; i < a_tetahedra_db.size(); i++)
		{
			arma::mat33 P;

			for (size_t j = 1; j < 4; j++)
			{
				a_tetahedra_db[i].P.col(j - 1) = arma::trans(arma::trans((a_vertices[a_tetahedra_db[i].vertices_index[j]] - a_vertices[a_tetahedra_db[i].vertices_index[0]])) * a_tetahedra_db[i].X);
			}

			arma::mat33 nabla_u = a_tetahedra_db[i].P - arma::eye(3, 3);
			arma::mat33 epsilon = 0.5* (nabla_u+arma::trans(nabla_u)+ arma::trans(nabla_u)*nabla_u);
			arma::mat33 sigma = E * epsilon;

			for (size_t findex = 0; findex < a_tetahedra_db[i].faces_index.size(); findex++)
			{
				arma::vec3 pj0 = a_vertices[a_tetahedra_db[i].vertices_index[0]];
				arma::vec3 pj1 = a_vertices[a_tetahedra_db[i].vertices_index[1]];
				arma::vec3 pj2 = a_vertices[a_tetahedra_db[i].vertices_index[2]];
				arma::vec3 fface = sigma * arma::cross(pj1-pj0,pj2-pj0);

				a_forces[a_tetahedra_db[i].vertices_index[0]] = fg + (1 / 3.0f)*fface;

			}
		}
		
		for (size_t k = 0; k < a_speed.size(); k++)
		{
			a_speed[k] += dt * a_forces[k] / m;
			a_vertices[k] += dt * a_speed[k];

		}

		//data copy
		//a_vertex_db_0.clear();
		//std::copy(a_vertex_db_1.begin(), a_vertex_db_1.end(), std::back_inserter(a_vertex_db_0));
		
		//update VAO
		tetamesh->vertex_mutex.lock();
		for ( size_t k=0; k<vertex_db.size(); k++)
		{
			vertex_db[k].position.x = a_vertices[k][0];
			vertex_db[k].position.y = a_vertices[k][1];
			vertex_db[k].position.z = a_vertices[k][2];
			f_vertices[k * 3 + 0] = a_vertices[k][0];
			f_vertices[k * 3 + 1] = a_vertices[k][1];
			f_vertices[k * 3 + 2] = a_vertices[k][2];

		}
		glNamedBufferSubData( tetamesh->VBO , 0x00 , sizeof(float)*f_vertices.size()*3 , a_vertices.data() );
		tetamesh->vertex_mutex.unlock();

		// update simulation run
		simulation_mutex.lock();
		running = simulation_running;
		simulation_mutex.unlock();

	}
}

TetahedraWindow::~TetahedraWindow()
{
}


