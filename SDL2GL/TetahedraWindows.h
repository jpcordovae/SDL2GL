#pragma once
#include "Window.h"
#include "TetahedraMesh.h"
#include <memory>
#include <thread>
#include "tetgen.h"
#include <mutex>
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

class TetahedraWindow : protected Window
{
public:
	typedef std::shared_ptr<TetahedraWindow> tetahedraWindowPtr;
	TetahedraWindow(int argc, char **argv);
	~TetahedraWindow();
	void Draw(float _dtime);
	void UpdateSimulation(double time_now);
	void LoadMeshFile(std::string meshfile);
private:
	std::thread thread_simulation;
	tetgenbehavior b;
	tetgenio in, out, addin, bgmin;
	tetgenmesh tmeshgen;
	bool simulation_running;
	std::mutex simulation_mutex;
	std::vector<stVertexData> vertex_db;
	meshPtr tetamesh = meshPtr(new Mesh());
	materialPtr matPtr = materialPtr(new Material());
	shaderPtr tetShader;
};

typedef TetahedraWindow::tetahedraWindowPtr tetahedraWindowPtr;

