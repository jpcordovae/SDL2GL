#pragma once
#include "Mesh.h"
#include "Shader.h"
#include <list>
#include <string>
#include <map>
#include <memory>

class renderable_exception : public std::runtime_error
{
public:
	renderable_exception(const std::string &error) :std::runtime_error(error) {};
};

class Renderable
{
public:
	typedef std::shared_ptr<Renderable> rendereablePtr;
	
	struct stShader {
		std::string shaderName;
		shaderPtr shader;
		stShader(std::string &_shaderName, shaderPtr _shader) : shaderName(_shaderName), shader(_shader) {}
	};

	struct stMesh {
		std::string meshName;
		meshPtr mesh;
	};

	Renderable();
	virtual ~Renderable();
	void deleteShader(const std::string &shaderName);
private:
	std::list<stShader> mStShaders;
	std::list<stMesh> mStrMeshs;
	
};

typedef Renderable::stMesh stMesh;
typedef Renderable::stShader stShader;
typedef Renderable::rendereablePtr renderablePtr;

Renderable::Renderable()
{
	mStShaders.push_back(stShader(	std::string("Difuse"),
									shaderPtr(new Shader("Shaders/diffuse_vertex.txt", "Shaders/diffuse_fragment.txt"))
								 )
						);
}

void Renderable::deleteShader(const std::string &shaderName)
{

}

Renderable::~Renderable()
{
}
