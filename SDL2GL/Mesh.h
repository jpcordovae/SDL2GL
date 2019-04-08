#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"
#include "Material.h"

struct stVertexData {
	stVertexData() :position(glm::vec3(0.0f,0.0f,0.0f)),
					normal(glm::vec3(0.0f, 0.0f, 0.0f)),
					tangent(glm::vec3(0.0f, 0.0f, 0.0f)),
					color(glm::vec3(0.0f, 0.0f, 0.0f)),
					mass(glm::vec3(1.0f,1.0f,1.0f)),
					velocity(glm::vec3(0.0f,0.0f,0.0f))
	{
		UV[0] = UV[1] = 0.0f;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 color;
	glm::vec3 mass;
	glm::vec3 velocity;

	float UV[2];
};

class mesh_exception : public std::runtime_error
{
public:
	mesh_exception(const std::string &error) :std::runtime_error(error) {};
};

class Mesh
{
public:
	enum mesh_display { TEXTURED, RAW_MESH, RAW_TRIANGLES, DIFFUSE_COLOR };
	typedef std::shared_ptr<Mesh> meshPtr;
	Mesh(std::vector<stVertexData> *vd, std::vector<unsigned int>*id, materialPtr material);
	void SetMeshData(std::vector<stVertexData> *vd, std::vector<unsigned int>*id, materialPtr material);
	void SetMeshData(std::vector<stVertexData> *vd, std::vector<unsigned int>*id);
	Mesh() {}
	~Mesh();
	void draw(unsigned int programId);
	void SetDisplayType(mesh_display md);
private:
	typedef struct {
		GLuint texture_index;
		GLuint texture_operation;
	} st_mesh_texture;

	mesh_display display_type;
	std::vector<stVertexData> data;
	materialPtr m_material; // only one material per mesh
	std::vector<unsigned int> indices;
	Material m_mat;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IND;
};

typedef Mesh::meshPtr meshPtr;
