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
#include <thread>
#include <mutex>
#include <armadillo.hpp>

class TetahedraWindow_exception : public std::runtime_error
{
public:
	TetahedraWindow_exception(const std::string &error) :std::runtime_error(error) {};
};

class TetahedraWindow : protected Window
{
public:

	typedef struct tetahedra {
		std::vector<arma::vec3> vertices;
		std::vector<arma::vec3> forces; // forces over vertices
		std::vector<unsigned int> vertices_index;
		std::vector<unsigned int> forces_index;
		std::vector<std::vector<unsigned int>> faces_index;
		arma::mat33 X;
		arma::mat33 P;
	} tetahedra;

	typedef std::shared_ptr<TetahedraWindow> tetahedraWindowPtr;

	TetahedraWindow(int argc, char **argv);
	~TetahedraWindow();
	void Draw(float _dtime);
	void InitSimulationData(void);
	void Initialization();

private:
	tetgenbehavior b;
	tetgenio in, out, addin, bgmin;
	tetgenmesh tmeshgen;
	std::vector<stVertexData> vertex_db;
	meshPtr tetamesh = meshPtr(new Mesh());
	meshPtr surfMesh = meshPtr(new Mesh());
	materialPtr matPtr = materialPtr(new Material());
	shaderPtr tetShader;
	shaderPtr tetSurfShader;
	// simulation stuffs
	bool simulation_running;
	std::vector<arma::vec3> a_vertices;
	std::vector<arma::vec3> a_forces;
	std::vector<tetahedra> a_tetahedra_db;
	std::vector<arma::vec3> a_vertex_db_0; // n 
	std::vector<arma::vec3> a_vertex_db_1; // n + 1
	std::thread simulation_thread;
	std::mutex simulation_mutex;
	void UpdateSimulation();
};

typedef TetahedraWindow::tetahedraWindowPtr tetahedraWindowPtr;

