#ifndef PARTICELS_H_
#define PARTICELS_H_

/*
#include <functional>
#include "Sprite.h"


// Boo, terrifying macros.
#define PARTICLE_FUNC std::function<void(Particle *)>
#define PARTICLE_RENDER_FUNC std::function<void(list<Particle*>&)>
#define DEFAULT_PARTICLE_FUNC [&](Particle *particle) {}
#define DEFAULT_PARTICLE_RENDER_FUNC [&](list<Particle*>& list) {}
// They are going to steal your candies, hehehe.


namespace pip
{
    //Particle Wonderful Effect (PWE).
	class ParticleEffect
	{
	public:
		// Constructor.
		ParticleEffect(Vector2D position,
				PARTICLE_FUNC emitFunc = DEFAULT_PARTICLE_FUNC,
				PARTICLE_FUNC updateFunc = DEFAULT_PARTICLE_FUNC,
				PARTICLE_RENDER_FUNC renderFunc = DEFAULT_PARTICLE_FUNC);
		// Destructor.
		~ParticleEffect();

		void emit(unsigned int count = 1);
		void update(double dt);
		void render();

	private:
		Vector2D position;

		struct Particle
		{
			pip::Color color;
			pip::Vector2D position;
			pip::Vector2D velocity;
		};
		list<Particle*> particles;

		PARTICLE_FUNC emitFunc, updateFunc;
		PARTICLE_RENDER_FUNC renderFunc;

		double totalElapsed = 0.0;
	};

}*/

#endif
