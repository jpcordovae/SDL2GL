#include "TetahedraWindows.h"

TetahedraWindow::TetahedraWindow()
{
	simulation_running = false;
	char switches[] = "tetgen.exe -q1.1Va1 -g example.poly";
	if (!b.parse_commandline(switches)) {
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

	tetrahedralize(&b, &in, NULL, &addin, &bgmin);

	std::vector<glm::vec3> points;
	for (int i = 0; i < in.numberofpointattributes; i++)
	{
		points.push_back(glm::vec3(in.pointlist[i * 3], in.pointlist[i * 3 + 1], in.pointlist[i * 3 + 2]));
	}
}

void TetahedraWindow::Draw(float _dtime)
{
	if (!this->IsInitialized() || !((bool)GetSDLWindowID())) return;
	SDL_GL_MakeCurrent(this->GetSDLWindow(),this->GetSDLContext());
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	
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


