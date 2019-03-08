#include "Material.h"
#include <iostream>
#include "Utils.h"

std::map<std::string, materialPtr> materials_db;

Material::Material(const aiMaterial *aiMat)
{
	assert(aiMat != nullptr);
	aiString matName;
	if (aiMat->Get(AI_MATKEY_NAME, matName) != aiReturn_SUCCESS)
	{
		//TODO: do something here, message and null the material
	}
	m_name = std::string(matName.C_Str());
	LoadTextures(aiMat);
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
		if (textures_db.find(std::string(path.C_Str())) != textures_db.end())
		{
			m_textures_diffuse = textures_db[std::string(path.C_Str())];
			continue;
		}
		textures_db[std::string(path.C_Str())] = texturePtr(new Texture(std::string(path.C_Str())));
		textures_db[std::string(path.C_Str())]->SetType(aiTextureType_DIFFUSE);
		m_textures_diffuse = textures_db[std::string(path.C_Str())];
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
