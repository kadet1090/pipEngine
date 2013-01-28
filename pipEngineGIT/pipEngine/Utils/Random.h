#ifndef RANDOM_H_
#define RANDOM_H_

/////////////////////////////////////////////////////////
////////		Some 'random' stuff				/////////
/////////////////////////////////////////////////////////

//Random stuff, haha!

#include <stdlib.h>
#include <time.h>

static bool randomDummy = []()->bool{srand((unsigned)time(0)); return true;}(); //It's kind of magic.

namespace pip
{
	namespace utils
	{
		inline float randomize()
		{
			return (float)rand()/(float)RAND_MAX;
		}

		template <typename T> T randrange(T min, T max)
		{
			return ((T)(rand()%(int)(max-min))+min);
		}

		inline float randrange(float min, float max)
		{
			return (((float)rand()/(float)RAND_MAX)*(max-min) + min);
		}

		inline double randrange(double min, double max)
		{
			return (((double)rand()/(double)RAND_MAX)*(max-min) + min);
		}
	}
}

#endif
