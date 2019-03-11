#pragma once
#include "Mesh.h"

class TetahedraMesh : public Mesh
{
public:
	TetahedraMesh(std::string file, std::string parameters);
	virtual ~TetahedraMesh();
};



