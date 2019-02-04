#pragma once
#include "Mesh.h"
#include "Material.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL_image.h>
#include <memory>
#include "Utils.h"

class scene_exception : public std::runtime_error
{
public:
	scene_exception(const std::string &error) :std::runtime_error(error) {};
};

class Scene
{
public:
	typedef std::shared_ptr<Scene> scenePtr;

	Scene(const char* filename);
	virtual ~Scene();
	void Draw(unsigned int programId);
	std::vector<meshPtr>& GetMeshes();
	bool existMaterial(std::string &matName);
	void readMaterialsFromScene(const aiScene* scene);
private:
	std::vector<meshPtr> meshes;
	void recursiveLoad(aiNode*, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void ReadAllMaterialsFromScene(const aiScene *scene);
	void LoadGLTextures(const aiScene* scene, const std::string& pModelPath);
	materialPtr ReadMaterial(const aiMaterial *mat);
	unsigned int loadTexture(const char *filename);
	GLuint*	textureIds = nullptr;// pointer to texture Array
	std::map < std::string, materialPtr> m_materials;
	std::map<std::string, uint16_t> m_textures;
};

typedef Scene::scenePtr scenePtr;

