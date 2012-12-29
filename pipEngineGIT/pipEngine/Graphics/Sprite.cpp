#include "Sprite.h"

pip::Sprite::Sprite(const string &name, Renderer *renderer, Shader *shader)
{
	// C++ doesn't let us to use 'this' pointer of outer class in inner class...
	this->width.parent = this;
	this->height.parent = this;
	this->animation.parent = this;

	this->load(name);
	this->renderer = renderer;
	this->shader = shader;
}

pip::Sprite::~Sprite()
{

}

void pip::Sprite::load(const string &name, const string &animation)
{
	std::cout << "Name: " << name << std::endl;
	this->myTextures[animation].frames.push_back(Texture::get(name, this->renderer));
	this->myTextures[animation].framesCount++;
}

void pip::Sprite::clear()
{
	this->totalElapsed = 1.0;
	this->frame = 0;
	this->myTextures.clear();
}

void pip::Sprite::setPixel(const Vector2D &position, const Color &color, unsigned int renderedFrame, const string &animation)
{
	this->unique(renderedFrame);
	//pip::this->renderer->setPixel(*this->myTextures[animation].frames[renderedFrame], position, color);
}

void pip::Sprite::render()
{
	if (this->myTextures[animation].framesCount)
	{
		pip::Texture *currentFrame = this->myTextures[this->animation].frames[this->frame];

		this->shader->bind();
		this->shader->setSampler("frameSampler", currentFrame);

		//currentFrame->bind();
		this->renderer->setColor(this->color);
		this->renderer->drawQuad(currentFrame->nearestWidth,currentFrame->nearestHeight,
				currentFrame->width,currentFrame->height,
				Vector2D(0,0),Vector2D(currentFrame->width,currentFrame->height),
				this->position,this->rotation);
	}
}

void pip::Sprite::update(double dt)
{
	//std::cout << "FRAME:" << this->frame << std::endl;
	//std::cout << "FRAMESCOUNT:" << this->framesCount << std::endl;
	if (this->paused)
	        return;
	if (this->myTextures[animation].framesCount)
	{
	    this->totalElapsed += dt;
	    if (this->totalElapsed > 1.0 / this->fps)
	    {
	    	std::cout << "Frame: " << this->frame << std::endl;
	        this->frame++;
	        if (this->frame  >= this->myTextures[animation].framesCount)
	        	this->frame = 0;
	        this->totalElapsed -= 1.0 / this->fps;
	        //std::cout << "Frame: " << this->frame << std::endl;
	    }
	}
}

void pip::Sprite::unique(unsigned int renderedFrame, const string &animation)
{
	if(this->myTextures[animation].frames[renderedFrame]->shared)
	{
		string temp = this->myTextures[animation].frames[renderedFrame]->name;

		this->myTextures[animation].frames[renderedFrame] = gc_new pip::Texture(this->renderer);
		this->myTextures[animation].frames[renderedFrame]->name = temp;
		this->myTextures[animation].frames[renderedFrame]->load(this->myTextures[animation].frames[renderedFrame]->name);
	}
}
