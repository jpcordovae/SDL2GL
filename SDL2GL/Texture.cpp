#include "Texture.h"
#include "Utils.h"
#include <assert.h>

std::map<std::string, Texture> mpTextures;

//this constructor is hiden to avoid empty textures
Texture::Texture()
{
	static_assert(1,"cannot call an empty constructor of this class.");
}

//This require a complete path to the file, check on blender export configuration 
// to enable this characteristic.
Texture::Texture(const std::string filename)
{
	//check filename
	if (!file_exist(filename.c_str()))
	{
		std::string out = "texture file " + filename + "doesn't exist";
		throw texture_exception(out.c_str());
	}
}

void Texture::SetID(unsigned int _id)
{
	id = _id;
}

unsigned int Texture::GetID()
{
	return id;
}

void Texture::SetType(unsigned int _type)
{
	type = _type;
}

unsigned int Texture::GetType()
{
	return type;
}

Texture::~Texture()
{

}
