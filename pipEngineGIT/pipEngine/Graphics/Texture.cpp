#include "Texture.h"
#include "Renderer.h"

std::map<pip::string,pip::Texture*> pip::Texture::textureMap;

//Delegation, FTW!
pip::Texture::Texture(Renderer *renderer)
{
	this->renderer = renderer;

	this->shared = true;
}

pip::Texture::~Texture()
{
	this->renderer->deleteTexture(this);
}

void pip::Texture::load(const std::string &name)
{
	this->renderer->deleteTexture(this);
	this->renderer->loadTexture(name, this);
	this->textureMap[name]->shared = false;
}

pip::Texture* pip::Texture::get(const std::string &name, Renderer *renderer)
{
	if(textureMap.count(name) == 0)
	{
		textureMap[name] = new pip::Texture(renderer);
		textureMap[name]->load(name);
		textureMap[name]->shared = true;
	}

	if(!textureMap[name]->id)
	{
		throw EngineException("Can't load texture '" + name + "'!"); // TODO Should it be checked when loading?
	}
	return textureMap[name];
}

void pip::Texture::bind()
{
	this->renderer->bindTexture(this);
}


//void pip::Texture::setPixel(Vector2D pos)
//{
//	this->renderer->setPixel(*this, pos);
//}



