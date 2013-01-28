/*
 * Color.h
 *
 *  Created on: 04-06-2012
 *      Author: szx
 */

#ifndef COLOR_H
#define COLOR_H

namespace pip
{
	struct Color
	{
		Color() { r = g = b = a = 0.0f; }

		Color(float r, float g, float b, float a = 1.0f)
		{
			this->r = r; 
			this->g = g; 
			this->b = b; 
			this->a = a;
		}

		Color operator +(const Color& c) const
		{
			return (Color(r + c.r, g + c.g, b + c.b));
		}

		Color operator -(const Color& c) const
		{
			return (Color(r - c.r, g - c.g, b - c.b));
		}

		Color operator *(const Color& c) const
		{
			return (Color(r * c.r, g * c.g, b * c.b));
		}

		Color operator /(const Color& c) const
		{
			return (Color(r / c.r, g / c.g, b / c.b));
		}

   		float r, g, b, a;
	};
}
#endif /* COLOR_H */
