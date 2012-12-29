#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <string>
#include <iostream>
//#include "Renderer.h"
#include "../Default.h"

namespace pip
{
	class Renderer;
	class Sprite;

	class Texture : public GCed
	{
	public:
		friend class Renderer;
		friend class Sprite;

		Texture(pip::Renderer *renderer);
		~Texture();
		void load(const string &name);
		void bind();
		//void setPixel(Vector2D pos);
		static Texture* get(const string &name, Renderer *renderer);

		unsigned int id;
		int width, height, nearestWidth, nearestHeight, channels = 0;
		pip::Renderer mutable *renderer;
	private:
		bool shared = false;
		string name;
		// Awful static map which is our texture manager. Pure evil.
		static std::map<string,pip::Texture*> textureMap;
	};
}

#endif
