#ifndef PIPMATH_H_
#define PIPMATH_H_

namespace pip
{

	inline int nearestPower(int value)
	{
		int pow = 1;
		while(pow < value)
			pow = pow << 1;
		return pow;
	}

	inline double abs(const double &a)
	{
		return (a<0.0?a*-1.0:a);
	}

}

#endif
