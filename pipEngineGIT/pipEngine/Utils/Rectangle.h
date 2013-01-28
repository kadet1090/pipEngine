#ifndef RECTANGLE_H_
#define RECTANGLE_H_


#include "Vector.h"
#include "Property.h"
#include "Math.h"

using pip::abs;

namespace pip
{

	struct Rectangle
	{
		//OH MY GOSH THEY USED MACRO!!!1!@1!oneoneone!@1!
		#define INIT_PROPS \
			this->width.get = [&](){return abs(this->end.x-this->start.x);}; \
			this->width.set = [&](double width){/*throw PropertyException();*/}; \
			 \
			this->height.get = [&](){return abs(this->end.y-this->start.y);}; \
			this->height.set = [&](double height){/*throw PropertyException();*/};


		Rectangle(Vector2D start, Vector2D end) : width(0), height(0)
		{
			this->start = start;
			this->end = end;

			INIT_PROPS //Awful! Why you've not just written new function, why?
		}

		Rectangle(Vector2D start, double width, double height) : width(0), height(0)
		{
			this->start = start;
			this->end = start + Vector2D(width, height);

			INIT_PROPS //I can't believe, that you've done this...
		}


		#undef INIT_PROPS //You'll suffer in hell. Coding in Visual Basic, buhehehe.


		Vector2D start, end;
		Property<double> width, height;
	};

}

#endif
