#include "Sprite.h"

pip::Sprite::Sprite(const string &name, Renderer *renderer, Shader *shader)
{
	this->load(name);
	this->renderer = renderer;
	this->shader = shader;

	this->totalElapsed = 0.0;

	this->color = Color(1.0,1.0,1.0,1.0);
	
    this->rotation = 0;
    this->rotationPoint = Vector2D(0.0, 0.0);

	this->position = Vector2D(0.0, 0.0);

    this->width.get     = [this]()->int { return this->animations[animation].frames[this->frame]->width; };
    this->height.get    = [this]()->int { return this->animations[animation].frames[this->frame]->height; };

    this->animation.set = [this](string value) {
        // reset animation-dependent data
		this->frame = 0;
		this->totalElapsed = 0;

		// set new animation
		if(this->animations.count(value) == 0)
			this->animation.value = "idle";
        else
		    this->animation.value = value;
    };

    this->animation = "idle";
}

pip::Sprite::~Sprite()
{

}

void pip::Sprite::load(const string &name, const string &animation)
{
	std::cout << "Name: " << name << std::endl;
	this->animations[animation].frames.push_back(Texture::get(name, this->renderer));
	this->animations[animation].framesCount++;
}

void pip::Sprite::clear()
{
	this->totalElapsed = 0.0;
	this->frame = 0;
	this->animations.clear();
}

void pip::Sprite::setPixel(const Vector2D &position, const Color &color, unsigned int renderedFrame, const string &animation)
{
	this->unique(renderedFrame);
	//pip::this->renderer->setPixel(*this->animations[animation].frames[renderedFrame], position, color);
}

void pip::Sprite::render()
{
	if (this->animations[animation].framesCount)
	{
		pip::Texture *currentFrame = this->animations[this->animation].frames[this->frame];

		if(this->shader) // if sprite is rendered by shader
		{
			this->shader->bind();
			this->shader->setSampler("frameSampler", currentFrame);
		}

		//currentFrame->bind();
		this->renderer->setColor(this->color);

		this->renderer->drawQuad(
			currentFrame->nearestWidth, currentFrame->nearestHeight,
			currentFrame->width, currentFrame->height,
			Vector2D(0,0), Vector2D(currentFrame->width, currentFrame->height),
            this->position, this->rotation, this->rotationPoint
		);
	}
}

// TODO: add fps multiplier
void pip::Sprite::update(double dt)
{
	//std::cout << "FRAME:" << this->frame << std::endl;
	//std::cout << "FRAMESCOUNT:" << this->framesCount << std::endl;

	if (this->paused)
	        return;

	pip::Sprite::AnimInfo* current = &this->animations[animation];

	if (current->framesCount)
	{
	    this->totalElapsed += dt;

	    if (this->totalElapsed > 1.0 / current->fps)
	    {
	        this->frame++;

			if (this->frame >= current->framesCount && current->looped)
				this->frame = current->loopStart;

			else if (this->frame >= current->framesCount && !current->looped)
            {
				this->paused = true;
                this->frame--;
            }

	        this->totalElapsed -= 1.0 / current->fps;
	    }
	}
}

void pip::Sprite::unique(unsigned int renderedFrame, const string &animation)
{
	if(this->animations[animation].frames[renderedFrame]->shared)
	{
		string temp = this->animations[animation].frames[renderedFrame]->name;

		this->animations[animation].frames[renderedFrame] = gc_new pip::Texture(this->renderer);
		this->animations[animation].frames[renderedFrame]->name = temp;
		this->animations[animation].frames[renderedFrame]->load(this->animations[animation].frames[renderedFrame]->name);
	}
}
