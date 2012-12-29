#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <functional>

#include "../Default.h"

namespace pip
{
	///Because we all hate accessors and mutators.
	template <class T>
	class Property
	{
	public:
		Property(T val)
		{
			this->val = val;
			this->get = [&](){return this->val;};
			this->set = [&](T val){this->val = val;};
		}
		~Property() {}

		Property& operator=(const Property &rhs)
		{
			if (this != &rhs)
			{
				this->set(rhs.get());
			}
			return *this;
		}
		Property& operator=(const T &rhs)
		{
			this->set(rhs);
			return *this;
		}
		operator T() const {return this->get();}


		std::function<T()> get;
		std::function<void(T)> set;
		T val;
	};

}

//How to use magic:
/*
 	Property<int> prop(0);
	prop.set = [&prop](int val){prop.val = val*2;};
	prop = 10;
	std::cout << prop << std::endl; //Prints 20

	prop.get = [&prop](){return prop.val/2.0f;};
	std::cout << prop << std::endl; //Prints 10

	Property<int> prop2 = 20 + prop;
	std::cout << prop2 << std::endl; //Prints 30
 */

#endif
