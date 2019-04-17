#pragma once
#include "Window.h"
#include <armadillo.hpp>
#include <mutex>
#include <thread>
#include "Mesh.h"

class PSBWindow_exception : public std::runtime_error
{
public:
	PSBWindow_exception(const std::string &error) :std::runtime_error(error) {};
};

class PBSWindows :	protected Window
{
public:
	// structures
	typedef std::shared_ptr<PBSWindows> pbswindowsPtr;
	// methods
	PBSWindows();
	void draw(float _dtime);
	void Initialization();
	virtual ~PBSWindows();
	// properties
	bool simulation_running;

};

typedef PBSWindows::pbswindowsPtr pbswindowsPtr;

