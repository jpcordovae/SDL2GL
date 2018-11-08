#include "stdafx.h"
#include "Scene.h"
#include <GL/glew.h>
#include "Material.h"

Scene::Scene(const char* filename)
{
	Assimp::Importer importer;
	
	const aiScene* scene = aiImportFile(filename,	aiProcess_GenSmoothNormals | 
													aiProcess_Triangulate | 
													aiProcess_CalcTangentSpace | 
													aiProcess_FlipUVs);
	
	if (!scene)
	{
		throw scene_exception(importer.GetErrorString());
	}

	LoadGLTextures(scene, getBasePath(filename));
	recursiveLoad(scene->mRootNode, scene);
	std::cout << "number of meshes : " << scene->mNumMeshes << std::endl;
	aiReleaseImport(scene);

}

void Scene::recursiveLoad(aiNode* node, const aiScene* scene)
{
	//recursion
#ifdef _DEBUG
	std::cout << "nNumChildren : " << node->mNumChildren << std::endl;
#endif
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		this->recursiveLoad(node->mChildren[i], scene);
	}
#ifdef _DEBUG
	std::cout << "nNumMeshes : " << node->mNumMeshes << std::endl;
#endif
	//process
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		//if (node->mMeshes != nullptr)
		{
			aiMesh* mesh = scene->mMeshes[i];
			this->processMesh(mesh, scene);
		}
	}

}

void Scene::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<stVertexData> data;
	std::vector<unsigned int> indices;
	std::vector<stTextureData> textures;
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		stVertexData tmp;
		glm::vec3 tmpVec;
		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		tmp.position = tmpVec;
		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		tmp.normal = tmpVec;

		if (mesh->mTangents)
		{
			tmpVec.x = mesh->mTangents[i].x;
			tmpVec.y = mesh->mTangents[i].y;
			tmpVec.z = mesh->mTangents[i].z;
		}
		else
		{
			tmpVec.x = 1.0;
			tmpVec.y = 0.0f;
			tmpVec.z = 0.0f;
		}
		tmp.tangent = tmpVec;

		if (mesh->mColors[0])
		{
			//TODO: what about the ALPHA !!!
			//mesh->mColors[0]->a ?? !!!!;
			tmpVec.x = mesh->mColors[0]->r;
			tmpVec.y = mesh->mColors[0]->g;
			tmpVec.z = mesh->mColors[0]->b;
		}
		else
		{
			tmpVec.x = tmpVec.y = tmpVec.z = 0.7f;
		}
		tmp.color = tmpVec;

		if (mesh->mTextureCoords[0])
		{
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
			//std::cout << "text coords: " << tmpVec.x << "," << tmpVec.y << std::endl;
		}
		else
		{
			tmpVec.x = tmpVec.y = tmpVec.z = 0.0;
		}
		tmp.UV[0] = tmpVec.x;
		tmp.UV[1] = tmpVec.y;
		data.push_back(tmp);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (size_t m = 0; m < scene->mNumMaterials; m++)
	{
		ReadMaterial(scene->mMaterials[m]);
	}

	/*aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	for (size_t i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		stTextureData tmp;
		tmp.id = loadTexture(str.C_Str());
		tmp.type = 0;
		textures.push_back(tmp);
	}*/
	meshes.push_back(meshPtr(new Mesh(&data,&indices,&textures)));
	
}

materialPtr Scene::ReadMaterial(const aiMaterial *mtl)
{
	return materialPtr(new Material(mtl));
}

void Scene::GetAllMaterialsFromScene(const aiScene *scene)
{
	for (size_t m = 0; m < scene->mNumMaterials; m++)
	{
		try {
			aiString name;
			scene->mMaterials[m]->Get(AI_MATKEY_NAME, name);
			std::map<std::string, materialPtr>::iterator it;
			it = materialContainer.find(std::string(name.C_Str()));
			if (it == materialContainer.end())
			{
				std::cerr << "material already loaded";
				continue;
			}
			materialContainer[std::string(name.C_Str())] = ReadMaterial(scene->mMaterials[m]);
		}
		catch (material_exception &e)
		{
			std::cerr << e.what();
			continue;
		}
	}
}

void Scene::LoadGLTextures(const aiScene* scene, const std::string& pModelPath)
{
	// Check if scene has textures.
	if (scene->HasTextures())
	{
		textureIds = new GLuint[scene->mNumTextures];
		glGenTextures(scene->mNumTextures, textureIds);// generate GL-textures ID's
		// upload textures
		for (size_t ti = 0; ti < scene->mNumTextures; ti++)
		{
			glBindTexture(GL_TEXTURE_2D, textureIds[ti]);// Binding of texture name
														 //
														 //redefine standard texture values
														 //
														 // We will use linear interpolation for magnification filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// tiling mode
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (scene->mTextures[ti]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (scene->mTextures[ti]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
			// Texture specification
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, scene->mTextures[ti]->mWidth, scene->mTextures[ti]->mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE,
				scene->mTextures[ti]->pcData);
		}
	}
}

unsigned int Scene::loadTexture(const char *filename)
{
	std::cout << "loading texture " << filename << std::endl;
	
	unsigned int num;
	GLCall(glGenTextures(1, &num));
	GLCall(glBindTexture(GL_TEXTURE_2D, num));
	
	std::string path = "C:\\Users\\jpcordovae\\Desktop\\SDL2GL\\SDL2GL\\3DModels\\";
	path += filename;
	SDL_Surface* img = IMG_Load(path.c_str());
	if (img == NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return -1;
	}

	int Mode = GL_RGB;

	if (img->format->BytesPerPixel == 4) {
		Mode = GL_RGBA8;
	}
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, Mode, img->w, img->h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	SDL_FreeSurface(img);
	return num;
}

void Scene::Draw(unsigned int programId)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw(programId);
	}
}

std::vector<meshPtr>& Scene::GetMeshes()
{
	return meshes; 
}

Scene::~Scene()
{
	//TODO: delete textures
}
