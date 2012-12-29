#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include "String.h"

namespace pip
{
	class InterpreterException : public std::exception
	{
	public:
		InterpreterException(const string &nval) {val = nval;}
		const char * what () const throw ()
		{
			return STR_CCHR(val);
		}
	private:
		string val;
	};
	class EngineException : public std::exception
	{
	public:
		EngineException(const string &nval) {val = nval;}
		const char * what () const throw ()
		{
			return STR_CCHR(val);
		}
	private:
		string val;
	};
}



#endif
