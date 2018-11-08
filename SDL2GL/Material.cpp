#include "Material.h"

std::map<std::string, materialPtr> materialContainer;// one cotainer for all the materials on the application

Material::Material(const aiMaterial *aiMat)
{
	assert(aiMat != nullptr);
	//LoadAiMaterial(aiMat);
	//std::map<std::string, materialPtr>::iterator matIt = materialContainer->find();
}

void Material::LoadTextures(const aiMaterial *mtl)
{
	//diffuse textures
	int texIndex = 0;
	aiString path;
	
	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_DIFFUSE); nTx++)
	{
		m_textures_diffuse[path.data] = NULL;
		texIndex++;
	}

	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_SPECULAR); nTx++)
	{
		m_textures_specular[path.data] = NULL;
		texIndex++;
	}

	for (size_t nTx = 0; nTx < mtl->GetTextureCount(aiTextureType_AMBIENT); nTx++)
	{
		m_textures_specular[path.data] = NULL;
		texIndex++;
	}
}

Material::~Material()
{

}
