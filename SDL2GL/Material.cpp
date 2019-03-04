#include "Material.h"
#include <iostream>
#include "Utils.h"
//#include <boost/log/trivial.hpp>

std::map<std::string, materialPtr> materialContainer;// one cotainer for all the materials on the application

Material::Material(const aiMaterial *aiMat)
{
	assert(aiMat != nullptr);
	aiString matName;
	if (aiMat->Get(AI_MATKEY_NAME, matName) != aiReturn_SUCCESS)
	{
		//TODO: do something here, message and null the material
	}
	m_name = std::string(matName.C_Str());
	//LoadAiMaterial(aiMat);
}

std::string Material::GetMaterialName()
{
	return m_name;
}

void Material::LoadTextures(const aiMaterial *mtl)
{
	int idx_texture = 0;
	aiString path;

	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_DIFFUSE); nTx++)
	{
		mtl->GetTexture(aiTextureType_DIFFUSE, idx_texture, &path);

		m_textures_diffuse[m_name] = (GLuint)idx_texture;
	}

	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_SPECULAR); nTx++)
	{
		m_textures_specular[m_name] = 0;
	}

	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_AMBIENT); nTx++)
	{
		m_textures_specular[m_name] = 0;
	}
}

GLuint Material::ReadTextureFromPath(const std::string &path)
{
	return 0;
}

Material::~Material()
{

}
