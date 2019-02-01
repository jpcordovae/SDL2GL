#pragma once
#include <memory>
#include <string>
#include <map>

struct stTextureData {
	stTextureData() :id(0), type(0) {}
	unsigned int id;
	unsigned int type;
};

class texture_exception : public std::runtime_error
{
public:
	texture_exception(const std::string &error) : std::runtime_error(error) {};
};

class Texture
{
public:
	typedef std::shared_ptr<Texture> texturePtr;
	Texture(const std::string filename);
	void SetID(unsigned int _id);
	unsigned int GetID();
	void SetType(unsigned int _type);
	unsigned int GetType();
	virtual ~Texture();
private:
	Texture();
	unsigned int id;
	unsigned int type;
};

typedef Texture::texturePtr texturePtr;
extern std::map<std::string,Texture> mpTextures;