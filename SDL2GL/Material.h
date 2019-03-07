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
	std::map<std::string, GLint> m_textures_diffuse;
	std::map<std::string, GLint> m_textures_specular;
	std::map<std::string, GLint> m_textures_ambient;
	std::map<std::string, GLint> m_textures_emissive;
	std::map<std::string, GLint> m_textures_height;
	std::map<std::string, GLint> m_textures_normals;
	std::map<std::string, GLint> m_textures_shininess;
	std::map<std::string, GLint> m_textures_opacity;
	std::map<std::string, GLint> m_textures_displacement;
	std::map<std::string, GLint> m_textures_lightmap;
	std::map<std::string, GLint> m_textures_reflection;
	std::map<std::string, GLint> m_textures_unknown;
};

typedef Material::materialPtr materialPtr;
