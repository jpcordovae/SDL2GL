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


class material_exception : public std::runtime_error
{
public:
	material_exception(const std::string &error) : std::runtime_error(error) {};
};

class stTexture {
private:
	std::string m_name;
	std::string m_path;
	int type;
	int id;
};

class Material
{
public:
	Material() {}
	typedef std::shared_ptr<Material> materialPtr;
	Material(const aiMaterial *aiMat);
	virtual ~Material();
private:
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
	std::map<std::string, stTexture*> m_mTextures;
	std::map<std::string, GLuint*> m_textures_diffuse;
	std::map<std::string, GLuint*> m_textures_specular;
	std::map<std::string, GLuint*> m_textures_ambient;
	std::map<std::string, GLuint*> m_textures_emissive;
	std::map<std::string, GLuint*> m_textures_height;
	std::map<std::string, GLuint*> m_textures_normals;
	std::map<std::string, GLuint*> m_textures_shininess;
	std::map<std::string, GLuint*> m_textures_opacity;
	std::map<std::string, GLuint*> m_textures_displacement;
	std::map<std::string, GLuint*> m_textures_lightmap;
	std::map<std::string, GLuint*> m_textures_reflection;
	std::map<std::string, GLuint*> m_textures_unknown;
};

typedef Material::materialPtr materialPtr;
extern std::map<std::string, materialPtr> materialContainer;
