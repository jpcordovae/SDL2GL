#pragma once
#include "Window.h"
#include "TetahedraMesh.h"
#include <memory>
#include <thread>
#include "tetgen.h"
#include <mutex>

class TetahedraWindow : protected Window
{
public:
	typedef std::shared_ptr<TetahedraWindow> tetahedraWindowPtr;
	TetahedraWindow();
	~TetahedraWindow();
	void Draw(float _dtime);
	void UpdateSimulation(double time_now);
	void LoadMeshFile(std::string meshfile);
private:
	std::thread thread_simulation;
	tetgenbehavior b;
	tetgenio in, addin, bgmin;
	bool simulation_running;
	std::mutex simulation_mutex;
};

typedef TetahedraWindow::tetahedraWindowPtr tetahedraWindowPtr;

