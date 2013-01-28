#ifndef VECTOR_
#define VECTOR_

#include <math.h>
#define PI 3.14159265

namespace pip
{

	struct Vector2D
	{
		double x,y;

		Vector2D() { x = y = 0; }

		Vector2D(double x, double y)
		{
			this->x = x; this->y = y;
		}

		#define OPERATOR(T1, action, T2) \
			T1 operator action (const T2& rhs) const {ACT(rhs,action);}


		#define ACT(rhs,action) return Vector2D(this->x action rhs.y, this->y action rhs.y);
		OPERATOR(Vector2D,+,Vector2D);
		OPERATOR(Vector2D,-,Vector2D);
		#undef ACT

		#define ACT(rhs,action) return Vector2D(this->x action rhs, this->y action rhs);
		OPERATOR(Vector2D,*,double);
		OPERATOR(Vector2D,/,double);
		#undef ACT

		#define ACT(rhs,action) return (this->x action rhs.x + this->y action rhs.y);
		OPERATOR(double,*,Vector2D);
		#undef ACT


		#define ASSIGN(type, action) \
			Vector2D& operator action ##=(const type& rhs) \
			{ \
				if (this != (Vector2D*)&rhs) { ACT(rhs, action); } return *this; \
			}


		#define ACT(rhs,action) \
				this->x action##= rhs.x; \
				this->y action##= rhs.y;
		ASSIGN(Vector2D,+);
		ASSIGN(Vector2D,-);
		#undef ACT

		#define ACT(rhs,action) \
				this->x action##= rhs; \
				this->y action##= rhs;
		ASSIGN(float,*);
		ASSIGN(float,/);
		#undef ACT


		#undef OPERATOR
		#undef ASSIGN
	};

	inline double Distance(const Vector2D &vec1, const Vector2D &vec2)
	{
		return sqrt(pow((vec1.x-vec2.x),2) + pow((vec1.y-vec2.y),2));
	}

	inline double LookAt(const Vector2D &vec1, const Vector2D &vec2)
	{
		return atan2((vec1.x - vec2.x), (vec1.y - vec2.y)) * (180.0f/PI);
	}

	inline void MoveLocal(Vector2D &vec, double rot, double dist)
	{
		double angle_radians = rot * PI/180.0f;
		vec.x +=  sin(angle_radians) * dist;
		vec.y +=  cos(angle_radians) * dist;
	}

}

#endif
