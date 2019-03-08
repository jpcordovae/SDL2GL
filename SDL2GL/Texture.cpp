#include "Texture.h"
#include "Utils.h"
#include <assert.h>
#include <SDL_image.h>

std::map<std::string, texturePtr> textures_db;

//this constructor is hiden to avoid empty textures
Texture::Texture()
{
	static_assert(1,"cannot call an empty constructor for this class.");
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
	load_texture_from_file(filename.c_str());
}


unsigned int Texture::load_texture_from_file(const char *filename)
{
	std::cout << "loading texture " << filename << std::endl;

	GLuint num;
	GLCall(glGenTextures(1, &num));
	GLCall(glBindTexture(GL_TEXTURE_2D, num));

	std::string path;// = "C:\\Users\\jpcordovae\\Desktop\\SDL2GL\\SDL2GL\\3DModels\\";
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
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	SDL_FreeSurface(img);
	SetID(num);
	return num;
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
