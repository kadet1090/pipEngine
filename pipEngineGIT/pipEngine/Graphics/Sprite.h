#ifndef SPRITE_H_
#define SPRITE_H_

#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "../Utils/GC.h"

namespace pip
{
    class Sprite; // Yay, crazy forward declarations.
	const Rectangle& getRectangle(const Sprite &sprite); //TODO Fuck out?

    // General Purpose Highly Functional PipEngine Sprite Class (GPHFPSC).
	class Sprite : public GCed
	{
	public:
		friend const Rectangle& getRectangle(const Sprite&);

		// Constructor.
		Sprite(const string &name, Renderer *renderer, Shader *shader = 0);
		// Destructor.
		~Sprite();
		// Loading our fantastic, beautiful graphic.
		void load(const string &name, const string &animation = "idle");
		// Clears our sprite;
		void clear();
		// Some pixel transformations - you know, ideal can be you can always improved!
		void setPixel(const Vector2D &position, const Color &color, unsigned int renderedFrame = 0, const string &animation = "idle");
		// Bring our marvelous sprite in front of player's eyes.
		void render();
		// Tell ours sprite to contemplate about sense of life, social role and its current animation's frame.
		void update(double dt);

		// Position...
		Vector2D position = Vector2D(0.0,0.0);
		// ...rotation...
		double rotation = 0;
		// ...and color of our sprite.
		Color color = Color(1.0,1.0,1.0,1.0);

		// Width and height.
		class {
			Sprite *parent; //C++ sux.
			public:
			friend class pip::Sprite;
		    operator unsigned int () const { return parent->myTextures[parent->animation].frames[parent->frame]->width; }
		} width; // Magic inside.
		class {
			Sprite *parent;
			public:
			friend class pip::Sprite;
		    operator unsigned int () const { return parent->myTextures[parent->animation].frames[parent->frame]->height; }
		} height;

		// Current animation's name ("idle" by default).
		class {
			Sprite *parent;
			string value = "idle";
			public:
			friend class pip::Sprite;
			string & operator = (const string &i)
			{
				parent->frame = 0;
				if(parent->myTextures.count(i) == 0) return value = "idle";
				return value = i;
			}
		    operator string () const { return this->value; }
		} animation;

		// Current animation's frame.
		unsigned int frame = 0;
		// Speed of our animation.
		double fps = 1.0;
		// Is animation paused?
		bool paused = false;

		Shader mutable *shader = 0;

	private:
		// Some private goodies.

		//Sprite's renderer.
		Renderer *renderer;

		// Make our texture unique.
		inline void unique(unsigned int renderedFrame = 0, const string &animation = "idle");

		//Evil implementation detail, don't touch.
		struct AnimInfo
		{
			vector<pip::Texture*> frames;
			unsigned int framesCount = 0;
		};
		// Don't touch too.
		map<string, AnimInfo> myTextures;

		double totalElapsed = 1.0;
	};

	inline const Rectangle& getRectangle(const Sprite &sprite)
	{
		return Rectangle(sprite.position - Vector2D(sprite.width/2.0, sprite.height/2.0), sprite.width, sprite.height);
	}

	inline bool isRectangleCollide(Rectangle a, Rectangle b)
	{
	    return (a.start.x + (a.width) > b.start.x
			&& a.start.x < b.start.x + (b.width)
			&& a.start.y + (a.height) > b.start.y
			&& a.start.y < b.start.y + (b.height));
	}
	inline bool isRectangleCollide(Sprite &a, Sprite &b)
	{
	   return isRectangleCollide(getRectangle(a), getRectangle(b));
	}
	inline bool isRectangleCollide(Rectangle a, Sprite &b)
	{
		return isRectangleCollide(a, getRectangle(b));
	}
	inline bool isRectangleCollide(Sprite &a, Rectangle b)
	{
		return isRectangleCollide(getRectangle(a), b);
	}
}

#endif
