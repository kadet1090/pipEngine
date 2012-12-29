#ifndef L_NUMBER_H_
#define L_NUMBER_H_

#include "../String.h"

namespace pip
{
	typedef double number;

	inline string numToStr(const number &num)
	{
		std::ostringstream s;
		s << num;
		return s.str();
	}
}

#endif
