#pragma once
#include <glm\glm.hpp>
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <map>
#include <memory>
#include <map>
#include <string>
#include <GL\glew.h>
#include "Texture.h"

class material_exception : public std::runtime_error
{
public:
	material_exception(const std::string &error) : std::runtime_error(error) {};
};

class Material
{
public:
	Material() {}
	typedef std::shared_ptr<Material> materialPtr;
	Material(const aiMaterial *aiMat);
	std::string GetMaterialName();
	virtual ~Material();
private:
	GLuint ReadTextureFromPath(const std::string &path);
	void LoadTextures(const aiMaterial *mtl);
	glm::vec4 m_color_ambient;
	glm::vec4 m_color_diffuse;
	glm::vec4 m_color_specular;
	glm::vec4 m_color_emissive;
	float m_shininess;
	float m_shininess_strength;
	bool m_bWireframe;
	bool m_bTwosided;
	std::string m_name; // same as in first of map on materialContainer
	texturePtr m_texture_diffuse;
	texturePtr m_texture_specular;
	texturePtr m_texture_ambient;
	texturePtr m_texture_emissive;
	texturePtr m_texture_height;
	texturePtr m_texture_normals;
	texturePtr m_texture_shininess;
	texturePtr m_texture_opacity;
	texturePtr m_texture_displacement;
	texturePtr m_texture_lightmap;
	texturePtr m_texture_reflection;
	texturePtr m_texture_unknown;

	friend class Mesh;
};

typedef Material::materialPtr materialPtr;
extern std::map<std::string, materialPtr> materials_db;
