/*
 * Primitives.h
 *
 *  Created on: 05-12-2012
 *      Author: szx
 */

#ifndef L_PRIMITIVES_H_
#define L_PRIMITIVES_H_

#include "../String.h"
#include "Number.h"
#include "../GC.h"
#include "../Exceptions.h"

#include <map>
namespace pip
{
	struct Atom; // Forward declaration.
	struct Env;
	typedef std::map<string, Atom*> map;


	enum Type {SYMBOL, STRING, NUMBER, LAMBDA, FEXPR, NIL, LIST, BUILTIN};


	class Atom : public GCed
	{
	public:
		Atom() : next(0){}
		virtual ~Atom(){}
		virtual Atom* eval(Env* env) = 0;
		virtual Atom* evalf(Env* env) = 0; //eval when first in list
		virtual Atom* evalq(Env* env) = 0; //eval quoted
		virtual bool isTrue(Env* env) = 0;
		virtual string repr() = 0;
		virtual Type type() = 0;
		Atom *next;
	};

	class Env : public GCed
	{
	public:
		Env(Env *nouter = 0) {outer = nouter;}
		Env* find(const string &var)
		{
			if(env.find(var) != env.end())
				return this;
			if(outer)
				return outer->find(var);
			throw InterpreterException("Unbounded symbol '" + var + "'");
		}
		Atom* get(const string &var)
		{
			if(env.find(var) != env.end())
				return env[var];
			throw InterpreterException("Unbounded symbol '" + var + "'");
		}
		void set(const string &var, Atom *atom)
		{
			env[var] = atom;
		}

		map env;
		Env *outer;
	};


	class Symbol : public Atom
	{
	public:
		Symbol(string nval) : val(nval) {}
		~Symbol() {}
		Atom* eval(Env* env)
		{
			Atom *atom = env->find(val)->get(val);
			atom->next = next;
			return atom->eval(env);
		}
		Atom* evalf(Env* env)
		{
			Atom *atom = env->find(val)->get(val);
			atom->next = next;
			return atom->evalf(env);
		}
		Atom* evalq(Env* env)
		{
			return this;
		}
		bool isTrue(Env* env) {return true;}
		Type type() {return SYMBOL;}
		string repr() {return val;}
		string val;
	};

	class String : public Atom
	{
	public:
		String(string nval) : val(nval) {}
		~String() {}
		Atom* eval(Env* env)
		{
			return this;
		}
		Atom* evalf(Env* env)
		{
			return this;
		}
		Atom* evalq(Env* env)
		{
			return this;
		}
		bool isTrue(Env* env) {return true;} //TODO False when empty?
		Type type() {return STRING;}
		string repr() {return val;}
		string val;
	};


	class Number : public Atom
	{
	public:
		Number(number nval) : val(nval) {}
		~Number() {}
		Atom* eval(Env* env)
		{
			return this;
		}
		Atom* evalf(Env* env)
		{
			return this;
		}
		Atom* evalq(Env* env)
		{
			return this;
		}
		bool isTrue(Env* env) {return val != number(0.0) ? true : false;}
		Type type() {return NUMBER;}
		string repr() {return numToStr(val);}
		number val;
	};

	class List : public Atom
	{
	public:
		List() {nextInList = 0;}
		~List() {}
		Atom* eval(Env* env)
		{
			if(type() != LIST && type() != NIL) // We have one element list (just normal object).
			{
				Atom *prevNext = nextInList->next;
				Atom *atom = nextInList->eval(env);
				atom->next = prevNext;
				return atom->eval(env);
			}
			else
				return evalf(env);
		}
		Atom* evalf(Env* env)
		{
			if(type() != NIL)
			{
				//std::cout << "List evalf" << std::endl;
				Atom *prevNext = nextInList->next;
				Atom *atom = nextInList->eval(env);
				atom->next = prevNext;
				return atom->evalf(env);
			}
			else
				return this;
		}
		Atom* evalq(Env* env)
		{
			if(type() != LIST && type() != NIL)
				return nextInList->evalq(env);
			else
				return this;
		}
		bool isTrue(Env* env) {return true;}
		Type type()
		{
			if(nextInList)
			{
				if(!nextInList)
					return nextInList->type();
				else
					return LIST;
			}else
			{
				return NIL;
			}

		}
		string repr()
		{
			if(nextInList)
			{
				if(!nextInList->next)
					return nextInList->repr();
				else
				{
					string repr("(");
					Atom **current = &nextInList;
					while(*current)
					{
						repr += (*current)->repr();
						repr += " ";
						current = &((*current)->next);
					}
					repr += string(")");
					return repr;
				}
			}else
				return string("NIL"); //TODO Caching or something.
		}
		Atom* nextInList;
	};

	class Lambda : public Atom
	{
	public:
		Lambda(List *nargs, List *nbody) : args(nargs), body(nbody), env(gc_new Env())
		{
			Atom *currentArg = args->nextInList;
			while(currentArg)
			{
				if(currentArg)
				{
					this->env->set(currentArg->repr(), new List());
					currentArg = currentArg->next;
				}
			}
		}
		~Lambda() {}
		Atom* eval(Env* env)
		{
			//std::cout << "Eval " << body->repr() << std::endl;
			this->env = env; //TODO I'm sure it's wrong way to implement closures.
			return this;
		}
		Atom* evalf(Env* env)
		{
			//std::cout << "Evalf " << body->repr() << std::endl;
			this->env->outer = env;

			Atom *currentGivenArg = this->next;
			Atom *currentArg = args->nextInList;
			if(currentArg && currentGivenArg) //TODO Arguments' type checking.
			{
				//std::cout << "as " << currentArg->repr() << std::endl;
				this->env/*->find(currentArg->repr())*/->set(currentArg->repr(), currentGivenArg->eval(this->env));
				currentArg = currentArg->next;
				currentGivenArg = currentGivenArg->next;
			}
			while(currentArg)
			{
				if(currentGivenArg)
				{
					this->env/*->find(currentArg->repr())*/->set(currentArg->repr(), currentGivenArg->eval(this->env));
					currentArg = currentArg->next;
					currentGivenArg = currentGivenArg->next;
				}
				else
				{
					throw InterpreterException("Too few arguments passed to lambda");
				}
			}

			body->next = next;
			//std::cout << "Body: " << body->repr() << std::endl;
			//std::cout << "..."<< std::endl;
			Atom *result = body->eval(this->env); //NEVER EVALF THIS!
			//std::cout << "End of " << body->repr() << std::endl;
			return result;
		}
		Atom* evalq(Env* env)
		{
			return this;
		}
		bool isTrue(Env* env) {return true;}
		Type type() {return LAMBDA;}
		string repr() {return string("lambda");}

		List *args;
		List *body;
		Env *env;
	};

	class Fexpr : public Atom //TODO Fuck out.
	{
	public:
		Fexpr(List *nargs, List *nbody) : args(nargs), body(nbody), env(gc_new Env())
		{
		}
		~Fexpr() {}

		Atom* eval(Env* env)
		{
			return this;
		}
		Atom* evalf(Env* env)
		{
			this->env->outer = env;

			Atom *currentGivenArg = this->next;
			Atom *currentArg = args->nextInList;
			if(currentArg && currentGivenArg) //TODO Arguments' type checking.
			{
				//std::cout << "as " << currentArg->repr() << std::endl;
				this->env->find(currentArg->repr())->set(currentArg->repr(), currentGivenArg);
				currentArg = currentArg->next;
				currentGivenArg = currentGivenArg->next;
			}
			while(currentArg)
			{
				if(currentGivenArg)
				{
					this->env->find(currentArg->repr())->set(currentArg->repr(), currentGivenArg);
					currentArg = currentArg->next;
					currentGivenArg = currentGivenArg->next;
				}
				else
				{
					throw InterpreterException("Too few arguments passed to fexpr");
				}
			}

			body->next = next;
			Atom *result = body->evalf(this->env);
			return result;
		}
		Atom* evalq(Env* env)
		{
			return this;
		}
		bool isTrue(Env* env) {return true;}
		Type type() {return FEXPR;}
		string repr() {return string("fexpr");}

		List *args;
		List *body;
		Env *env;
	};


	typedef Atom* (*BuiltInFunc)(Env*, Atom*);
	typedef std::map<string, BuiltInFunc> funcMap;

	class BuiltIn : public Atom
	{
	public:
		BuiltIn(Env *nenv, const string &nname) : name(nname) {env = nenv;}
		~BuiltIn() {}

		static void add(const string &name, BuiltInFunc func)
		{
			map[name] = func;
		}
		Atom* eval(Env *env)
		{
			return this;
		}
		Atom* evalf(Env *env)
		{
			//std::cout << name << std::endl;
			return map[name](env, next);
		}
		Atom* evalq(Env *env)
		{
			return this;
		}
		bool isTrue(Env* env) {return true;}
		Type type() {return BUILTIN;}
		string repr() {return string("builtIn");}

		static funcMap map;

		Env *env;
		string name;
	};

	funcMap BuiltIn::map;
}


#endif
