#ifndef SPRITE_H_
#define SPRITE_H_

#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "../Utils/GC.h"
#include "../Utils/Property.h"

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
		    Vector2D position;
		    // ...rotation...
		    double rotation;
		    // ...and color of our sprite.
		    Color color;

		    // Width and height.
		    Property<int> width;
            Property<int> height;

            // current animation name
		    Property<string> animation;

		    // Current animation's frame.
		    unsigned int frame;
		    // Multiplier of animation speed.
		    double fpsMultiplier;
		    // Is animation paused?
		    bool paused;

		    Shader mutable *shader;
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
			    unsigned int framesCount;

			    bool looped;
			    int loopStart;
			    int fps;

                AnimInfo() : looped(false), loopStart(0), fps(10), framesCount(0) { }
		    };

		    // Don't touch too.
		    map<string, AnimInfo> animations;

		    double totalElapsed;

        public:
            AnimInfo* operator[] (const string &name)
            {
                return &this->animations[name];
            }
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
