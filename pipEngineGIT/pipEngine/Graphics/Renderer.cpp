#include "Renderer.h"

std::map<pip::string, std::map<float, OGLFT::MonochromeTexture*>> pip::Renderer::facesMap;
//pip::Renderer *pip::globalRenderer = 0;

pip::Renderer::Renderer(unsigned int width, unsigned int height)
{
	//if(globalRenderer)
	//	throw std::runtime_error("Yeah, try make second renderer, please, do it...");
	//pip::globalRenderer = this;

	this->width = width;
	this->height = height;

	//this->pixel = new Texture(this);
	//this->makeTexture(this->pixel, Rectangle(Vector2D(0.0,0.0),1.0,1.0), Color(1.0,1.0,1.0,1.0));

    //glClearDepth(1.f);

    //glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);

    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.1);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_SCISSOR_TEST);

    glGenFramebuffers(1, &this->fboId);

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // Not even sure, why I need this.

    if (!(GL_ARB_vertex_shader && GL_ARB_fragment_shader))
    	throw EngineException("GLSL not supported!");
}

pip::Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &this->fboId);
	//if(globalRenderer != this)
	//		throw std::runtime_error("I'm trying to clean all the mess you did with your renderer and what I see? Someone changed address pointed by globalRendered. Damn, I hate you.");
}


void pip::Renderer::begin() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void pip::Renderer::end() const
{
	/*std::cout << "Before trap." << std::endl;

	this->setTarget();

    this->drawQuad(nearestPower(this->width),nearestPower(this->height),
    		this->width,this->height,
    		Vector2D(0,0),Vector2D(this->width,this->height),
    		Vector2D(this->width/2.0,this->height/2.0),0);
    std::cout << "After trap." << std::endl;*/
	//glAccum(GL_MULT, 0.90);
	//glAccum(GL_ACCUM, 0.10);
	//glAccum(GL_RETURN, 1.0);

	glFlush();

}


void pip::Renderer::prepareProjection(unsigned int width, unsigned int height) const
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void pip::Renderer::setRegion(const Rectangle &area) const
{
	glScissor(area.start.x,area.start.y,area.end.x-area.start.x,area.end.y-area.start.y);
}

void pip::Renderer::setClearColor(const Color &color) const
{
	glClearColor(color.r,color.g,color.b,color.a);
}

void pip::Renderer::clearToColor() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pip::Renderer::setColor(const Color &color) const
{
	//this->currentColor = color;
	glColor4f(color.r,color.g,color.b,color.a);
}

void pip::Renderer::loadTexture(const std::string &name, Texture *texture) const
{
	// You are not expected to understand this.
	texture->name = name;
	texture->id = 0;
	texture->width = texture->height = texture->channels = 0;

    std::cout << "NAME: " << texture->name << std::endl;
    unsigned char *imageData = SOIL_load_image
    		(
    				name.c_str(),
    				&texture->width, &texture->height, &texture->channels,
    				0
    		);

    if(!imageData)
    	return;

    std::cout << texture->width << " " << texture->height << " " << texture->channels << std::endl;

    texture->id = 0;
    glGenTextures(1,&texture->id);
    std::cout << "Texture number: " << texture->id << std::endl;

    int upperWidth = pip::nearestPower(texture->width);
    int upperHeight = pip::nearestPower(texture->height);
    texture->nearestWidth = (int)upperWidth;
    texture->nearestHeight = (int)upperHeight;

    std::cout << "upperWidth: " << upperWidth << std::endl;
    std::cout << "upperHeight: " << upperHeight << std::endl;

    unsigned char* newImageData = 0;

    if(upperWidth != texture->width || upperHeight != texture->height)
    {
        unsigned char* newImageData = reinterpret_cast<unsigned char*>(malloc(sizeof(unsigned char) * upperWidth * upperHeight * texture->channels));
        memset(newImageData,0,sizeof(unsigned char) * upperWidth * upperHeight * texture->channels);
        for(int y = 0; y < texture->height; ++y)
        memcpy(&newImageData[y * upperWidth * texture->channels], &(imageData[y * texture->width * texture->channels]), texture->width * texture->channels);

        texture->bind();

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, upperWidth, upperHeight, GL_RGBA, GL_UNSIGNED_BYTE, newImageData);
        SOIL_free_image_data(newImageData);
    }else
    {
        newImageData = imageData;
        texture->bind();
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, upperWidth, upperHeight, GL_RGBA, GL_UNSIGNED_BYTE, newImageData);
    }

    SOIL_free_image_data(imageData);
    std::cout << "Texture number: " << texture->id << std::endl;

}


/*void pip::Renderer::makeTexture(Texture &texture, const Rectangle &area, const Color &color) const
{
	//TODO Worst code ever?
	texture.width = area.width;
	texture.height = area.height;
	texture.channels = 4;

	std::cout << texture.width << " " << texture.height << " " << texture.channels << std::endl;

	texture.data = (void*)new unsigned int;
	glGenTextures(1,(unsigned int*)texture.data);
	std::cout << "Texture number: " << *((unsigned int*)texture.data) << std::endl;

	int upperWidth = pip::nearestPower(texture.width);
	int upperHeight = pip::nearestPower(texture.height);
	texture.nearestWidth = (int)upperWidth;
	texture.nearestHeight = (int)upperHeight;

	std::cout << "upperWidth: " << upperWidth << std::endl;
	std::cout << "upperHeight: " << upperHeight << std::endl;

	unsigned char* imageData = reinterpret_cast<unsigned char*>(malloc(sizeof(unsigned char) * upperWidth * upperHeight * texture.channels));
	memset(imageData,0,sizeof(unsigned char) * upperWidth * upperHeight * texture.channels);
	for(unsigned int i = 0; i < sizeof(unsigned char) * upperWidth * upperHeight * texture.channels;i+=texture.channels)
	{
		imageData[i] = static_cast<unsigned char>(color.r*255.0);
		imageData[i+1] = static_cast<unsigned char>(color.g*255.0);
		imageData[i+2] = static_cast<unsigned char>(color.b*255.0);
		imageData[i+3] = static_cast<unsigned char>(color.a*255.0);
	}

	texture.bind();

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, upperWidth, upperHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	SOIL_free_image_data(imageData);

	std::cout << "Texture number: " << *(unsigned int*)texture.data << std::endl;
}*/

void pip::Renderer::bindTexture(Texture *texture) const
{
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void pip::Renderer::setPixel(Texture *texture, const Vector2D &position, const Color &color)
{
	Texture *previousTarget = this->currentTarget;
	this->setTarget(texture);
	this->pixel->bind();
	this->setColor(color);
	this->drawQuad(1,1,1,1,Vector2D(0.0,0.0),Vector2D(1.0,1.0),Vector2D(position.x,(double)texture->height-position.y),0.0);
	if(previousTarget)
		this->setTarget(previousTarget); //TODO Target stack?
	else
		this->setTarget();
}

void pip::Renderer::deleteTexture(Texture *texture) const
{
	std::cout << "Deleting..." << std::endl;
	glDeleteTextures(1, &texture->id);
}


void pip::Renderer::setTarget(Texture *texture)
{
	this->currentTarget = texture;
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
    this->prepareProjection(texture->width,texture->height);
	this->setRegion(Rectangle(Vector2D(0.0,0.0),Vector2D(texture->width,texture->height)));
}

void pip::Renderer::setTarget()
{
	this->currentTarget = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->prepareProjection(this->width, this->height);
	this->setRegion(Rectangle(Vector2D(0.0,0.0), Vector2D((double)this->width,(double)this->height)));
}


void pip::Renderer::drawQuad(unsigned int width, unsigned int height,
                             unsigned int textureWidth, unsigned int textureHeight, 
                             Vector2D textureStart, Vector2D textureEnd,
                             Vector2D position, float angle, Vector2D axis) const
{
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef((float)width, -(float)height, 0.0f);

	if(GLEE_ARB_vertex_buffer_object)
	{
		//TODO Code it.
	}

	//Immediate mode - slow.
	glBegin(GL_QUADS); //TODO: Vertex Array (Compiled Vertex Array, VBO?)
		    glTexCoord2d((double)textureStart.x/(double)textureWidth, 1.0 - (double)textureStart.y/(double)textureHeight); glVertex3f(-0.5f, -0.5f,  0.0f);  // Bottom Left Of The Texture and Quad
		    glTexCoord2d((double)textureEnd.x/(double)textureWidth, 1.0 - (double)textureStart.y/(double)textureHeight); glVertex3f( 0.5f, -0.5f,  0.0f);  // Bottom Right Of The Texture and Quad
		    glTexCoord2d((double)textureEnd.x/(double)textureWidth, 1.0 -(double)textureEnd.y/(double)textureHeight); glVertex3f( 0.5f,  0.5f,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2d((double)textureStart.x/(double)textureWidth, 1.0- (double)textureEnd.y/(double)textureHeight ); glVertex3f(-0.5f,  0.5f,  0.0f);  // Top Left Of The Texture and Quad
		    /*
		    glTexCoord2d((double)textureStart.x/(double)textureWidth, (double)textureStart.y/(double)textureHeight); glVertex3f(-0.5f, -0.5f,  0.0f);  // Bottom Left Of The Texture and Quad
		    glTexCoord2d((double)textureEnd.x/(double)textureWidth, 0.0f); glVertex3f( 0.5f, -0.5f,  0.0f);  // Bottom Right Of The Texture and Quad
		    glTexCoord2d((double)textureEnd.x/(double)textureWidth, (double)textureEnd.y/(double)textureHeight); glVertex3f( 0.5f,  0.5f,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2d(0.0f,(double)textureEnd.y/(double)textureHeight); glVertex3f(-0.5f,  0.5f,  0.0f);  // Top Left Of The Texture and Quad
		     */
	glEnd();
	glPopMatrix();
}

void pip::Renderer::drawText(Vector2D position, const string &text, const string &name, float size) const
{
	if(this->facesMap.count(name) == 0)
	{
		this->facesMap[name][size] = new OGLFT::MonochromeTexture(name.c_str(), size);
		if (this->facesMap[name][size] == 0 || !this->facesMap[name][size]->isValid())
			throw "Can't load '" + name + "' font!";
		this->facesMap[name][size]->setCharacterRotationX(180.0f);
	}
	this->facesMap[name][size]->draw(position.x, position.y, text.c_str());

}

