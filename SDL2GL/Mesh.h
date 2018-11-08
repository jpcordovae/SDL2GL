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
					color(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		UV[0] = UV[1] = 0.0f;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 color;
	float UV[2];
};

class Mesh
{
	std::vector<stVertexData> data;
	std::vector<stTextureData> textures;
	std::vector<unsigned int> indices;
	Material m_mat;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IND;
	unsigned int TXR; // for textures indexs
public:
	typedef std::shared_ptr<Mesh> meshPtr;
	Mesh(std::vector<stVertexData> *vd, std::vector<unsigned int>*id, std::vector<stTextureData> *td);
	~Mesh();
	void draw(unsigned int programId);
};

typedef Mesh::meshPtr meshPtr;
