#include "TetahedraWindows.h"

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
	}

	else { // -p
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

	std::cout << "numberofpoints: " << out.numberofpoints << "\n";

	std::vector<stVertexData> vertex_db;
	stVertexData vd;
	vertex_db.push_back(vd);
	for (int i = 0; i < out.numberofpoints; i++)
	{
		vd.position = glm::vec3(out.pointlist[i * 3], out.pointlist[i * 3 + 1], out.pointlist[i * 3 + 2]);
		vertex_db.push_back(vd);
	}

	std::cout << "numberoftrifaces: " << out.numberoftrifaces << "\n";

	std::vector<unsigned int> faceindex;
	for (size_t i = 0; i < out.numberoftrifaces; i++)
	{
		/*if (i < 20)
		{
			std::cout << "    " << i << ": trifacelist "
				<< out.trifacelist[i * 3] << " " << out.trifacelist[i * 3 + 1] << " " << out.trifacelist[i * 3 + 2]
				<< "\n";
		}*/
		faceindex.push_back((unsigned int)out.trifacelist[i*3]);
		faceindex.push_back((unsigned int)out.trifacelist[i*3 + 1]);
		faceindex.push_back((unsigned int)out.trifacelist[i*3 + 2]);
	}

	std::cout	<< "numberoftetrahedra: "	<< out.numberoftetrahedra
				<< " numberofcorners: "		<< out.numberofcorners
				<< " firstnumber: "			<< out.firstnumber << "\n";

	std::vector<unsigned int> trindex;
	for (size_t i = 0; i < out.numberoftetrahedra; i++)
	{
		/*if (i < 20)
		{
			std::cout << "    " << i << ": tetrahedronlist "
				<< out.tetrahedronlist[i * 4 + 0] << " "
				<< out.tetrahedronlist[i * 4 + 1] << " "
				<< out.tetrahedronlist[i * 4 + 2] << " "
				<< out.tetrahedronlist[i * 4 + 3] << "\n";
		}*/

		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);

		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);

		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 2]);

		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 1]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 3]);
		trindex.push_back(out.tetrahedronlist[i * 4 + 0]);

	}

	tetamesh->SetMeshData(&vertex_db, &trindex);
	tetamesh->SetDisplayType(Mesh::RAW_MESH);
	this->sdlCamera->setLocation(glm::vec3(0.0f, 0.0f, 300.0f));
}

void TetahedraWindow::Draw(float _dtime)
{
	if (!this->IsInitialized() || !((bool)GetSDLWindowID())) return;
	SDL_GL_MakeCurrent(this->GetSDLWindow(),this->GetSDLContext());
	// glewInit must be called after each context change
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		exit(1);
	}
	//
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//
	float rotAngle = 0.0f;
	//glm::mat4 model = glm::rotate(rotAngle += 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//sdlCamera->setLocation(glm::vec3(0.0f, 0.0f, 300.0f));
	glm::mat4 view = glm::lookAt(sdlCamera->getVector(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * GetWindowWidth() / GetWindowHeight(), 0.1f, 100000.0f);
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProjectionMatrix = projection * view * model;
	glm::mat3 normalMatrix = glm::mat3(modelView);

	this->tetShader->Use();

	int programId = this->tetShader->GetShaderProgram();

	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix)));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix)));

	GLCall(glUniform3f(glGetUniformLocation(programId, "cameraPosition"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "lightPos"), sdlCamera->getLocation().x, sdlCamera->getLocation().y, sdlCamera->getLocation().z));
	GLCall(glUniform3f(glGetUniformLocation(programId, "ambientColor"), 0.5f,0.5f,0.5f));

	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//this->scene->Draw(this->winShader->GetShaderProgram());
	tetamesh->draw(programId);
	SDL_GL_SwapWindow(GetSDLWindow()); // swap buffers
}

void TetahedraWindow::LoadMeshFile(std::string meshfile)
{
	
}

void TetahedraWindow::UpdateSimulation(double time_now)
{
	/*void tetrahedralize(char *switches,
						tetgenio *in,
						tetgenio *out,
						tetgenio *addin,
						tetgenio *bgmin)*/

	bool running;
	simulation_mutex.lock();
	simulation_running = true;
	running = simulation_running;
	simulation_mutex.unlock();

	while (running) 
	{
		//update simulation
		//fast data copy
		//update VAO
		simulation_mutex.lock();
		running = simulation_running;
		simulation_mutex.unlock();
	}
}

TetahedraWindow::~TetahedraWindow()
{
}


