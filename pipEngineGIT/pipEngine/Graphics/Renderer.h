#ifndef RENDERER_H_
#define RENDERER_H_

#define GC X11_GC
#define Status X11_Status
#define None X11_None
#include "GLee.h"
#undef None //Why, you ask?
#undef Status //XLib declares this names as macros and typedefs.
#undef GC //What a shame...

#include <GL/glu.h>
#include <SOIL.h>

#define OGLFT_NO_SOLID // I forgot to compile OGLFT without that stuff...
#define OGLFT_NO_QT // Yep, I'm too lazy to fix it.
#include <OGLFT.h>

#include "Texture.h"

#include "../Default.h"
#include "../Utils/Rectangle.h"
#include "Color.h"

#include <string.h> //memcpy
#include <stdlib.h> //malloc, free

namespace pip
{
    inline void getOGLErrors(string &str)
    {
    	GLenum glErr;
    	glErr = glGetError();
    	while (glErr != GL_NO_ERROR)
    	{
    		str += string((const char *)gluErrorString(glErr)) + '\n';
    		glErr = glGetError();
    	}
    }

	class Renderer : public GCed
	{
	public:
		// I'm lazy, so I won't describe this huddle of methods.
		Renderer(unsigned int width, unsigned int height);
		~Renderer();

		void begin() const;
		void end() const;

		void prepareProjection(unsigned int width, unsigned int height) const;
		unsigned int getHeight() const {return this->height;}
		unsigned int getWidth() const {return this->width;}
		void setHeight(unsigned int height) {this->height = height;}
		void setWidth(unsigned int width) {this->width = width;}

		void setRegion(const Rectangle &area) const;
		void clearToColor() const;

		void setClearColor(const Color &color) const;
		void setColor(const Color &color) const;

		void loadTexture(const std::string &name, Texture *texture) const;
		//void makeTexture(Texture &texture, const Rectangle &area, const Color &color) const;
		void bindTexture(Texture *texture) const;
		void setPixel(Texture *texture, const Vector2D &position, const Color &color);
		void deleteTexture(Texture *texture) const;

		void setTarget();
		void setTarget(Texture *texture);

		void drawQuad(unsigned int width, unsigned int height, unsigned int textureWidth, unsigned int textureHeight, Vector2D textureStart, Vector2D textureEnd, Vector2D position, float angle) const;
		void drawText(Vector2D position, const string &text, const string &name, float size = 12) const;

	private:
		// Some low-level stuff. Don't care.
		unsigned int width, height;

		GLuint fboId;

		Texture *currentTarget;
		Texture *pixel;
		Color currentColor;

		// Static map of maps to pointers to faces. Yeah...
		static std::map<string, std::map<float, OGLFT::MonochromeTexture*>> facesMap;
	};

	//Yes, /me did this.
	//extern Renderer *globalRenderer;
	//F*ck singletons, I'm going to use language's build-in features.
}


#endif
