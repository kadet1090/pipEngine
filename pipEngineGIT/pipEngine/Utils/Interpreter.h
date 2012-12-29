#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <iostream>
#include "Interpreter/Primitives.h"

namespace pip
{
	Atom* makeAST(tokensVector &tokens, Env *env, unsigned int *counter = 0) // TODO Refactor.
	{
		if(counter == 0)
		{
			//std::cout << "Start" << std::endl;
			List *prog = gc_new List();
			prog->nextInList = gc_new BuiltIn(env, string("begin"));
			prog->nextInList->next = makeAST(tokens, env, new unsigned int(0));
			delete counter;
			//std::cout << "KJRWUa" << std::endl;
			return prog;
		}
		else
		{
			number val = 0.0;
			if(from_string<number>(val, tokens[(*counter)].val, std::dec)) // Number
			{
				//std::cout << "Number" << std::endl;
				(*counter)++;
				List *atom = gc_new List();
				atom->nextInList = gc_new Number(val);
				return (List*)atom;
			}else if(tokens[(*counter)].val == "(") // List
			{
				if(tokens[(*counter)+1].val == "lambda")
				{
					//std::cout << "Lambda" << std::endl;(lambda (x y) (+ x y))
					//List *args = gc_new List();
					*counter += 2;
					List *args = (List*)makeAST(tokens, env, counter);
					List *body = (List*)makeAST(tokens, env, counter);
					//body->nextInList = makeAST(tokens, env, counter);
					(*counter)++;
					return gc_new Lambda(args, body);
				}else if(tokens[(*counter)+1].val == "fexpr")
				{
					//std::cout << "Fexpr" << std::endl;
					*counter += 2;
					List *args = (List*)makeAST(tokens, env, counter);
					List *body = (List*)makeAST(tokens, env, counter);
					(*counter)++;
					return gc_new Fexpr(args, body);
				}else if(tokens[(*counter)+1].val == "built-in")
				{
					//std::cout << "BuiltIn" << std::endl;
					BuiltIn* atom;
					if(tokens[(*counter)+2].val != ")") //TODO Jeez, array size checking...
						atom = gc_new BuiltIn(env, string((tokens[(*counter)+2].val)));
					else
						throw InterpreterException("Built-in without argument"); //TODO My English sucks.
					if((tokens[(*counter)+3].val != ")"))
						throw InterpreterException("Built-in with too many arguments"); //TODO Special type for parsing errors.

					(*counter) += 4;
					return atom;
				}else
				{
					//std::cout << "List" << std::endl;
					List* atom = gc_new List();
					(*counter)++;
					if(tokens[(*counter)].val == ")") // Empty
					{
						(*counter)++;
						return atom;
					}else
					{
						atom->nextInList = makeAST(tokens, env, counter);
						//std::cout << tokens[(*counter)].val << std::endl;
						Atom **currentAtom = &atom->nextInList->next;
						while(tokens[(*counter)].val != ")")
						{
							//std::cout << "AAAA" << std::endl;
							*currentAtom = makeAST(tokens, env, counter);
							currentAtom = &((*currentAtom)->next);
						}
						(*counter)++;
						return atom;
					}
				}
			}else //Symbol
			{
				if(tokens[(*counter)].quoted == false) //Symbol
				{
					//std::cout << "Symbol " << tokens[(*counter)].val << std::endl;
					(*counter)++;
					return gc_new Symbol(tokens[(*counter-1)].val);
				}else //String
				{
					//std::cout << "String " << tokens[(*counter)].val << std::endl;
					(*counter)++;
					return gc_new String(tokens[(*counter-1)].val);
				}
			}
		}
	}




	Atom* beginProc(Env* env, Atom* rest)
	{
		//std::cout << "Begin proc" << std::endl;(define count-down-from (lambda (n) (lambda () ((begin ((set n (- n 1)) n))) )))
		Atom *result = new List();
		if(rest)
		{
			if(rest->type() == LIST)
			{
				Atom *current = ((List*)rest)->nextInList;
				while(current->next)
				{
					std::cout << current->repr() << std::endl;
					current->evalf(env);
					current = current->next;
				}
				result = current->evalf(env);
			}else
			{
				throw InterpreterException("Wrong argument passed to 'begin' built-in");
			}
		}
		return result;
	}


	Atom* addProc(Env* env, Atom* rest)
	{
		//std::cout << "Add proc" << std::endl;
		Number *result = new Number(number(0.0));
		Number *current = (Number*)rest;
		while(current)
		{
			Number *atom = (Number*)current->eval(env);
			if(atom->type() == NUMBER)
			{
				result->val += atom->val;
				current = (Number*)current->next;
			}
			else
			{
				throw InterpreterException("Cannot sum non-numbers.");
			}
		}
		return result;
	}

	Atom* mulProc(Env* env, Atom* rest)
	{
		Number *result = new Number(number(0.0));
		Number *current = (Number*)rest;
		if(current)
		{
			Number *atom = (Number*)current->eval(env);
			if(atom->type() == NUMBER)
			{
				result->val = atom->val;
				current = (Number*)current->next;
			}else
			{
				throw InterpreterException("Cannot multiple non-numbers.");
			}
			while(current)
			{
				Number *atom = (Number*)current->eval(env);
				if(atom->type() == NUMBER)
				{
					result->val = result->val * atom->val;
					current = (Number*)current->next;
				}
				else
				{
					throw InterpreterException("Cannot multiple non-numbers.");
				}
			}
		}
		return result;
	}
	Atom* subProc(Env* env, Atom* rest)
	{
		Number *result = new Number(number(0.0));
		Number *current = (Number*)rest;
		if(current)
		{
			Number *atom = (Number*)current->eval(env);
			if(atom->type() == NUMBER)
			{
				result->val = atom->val;
				current = (Number*)current->next;
			}else
			{
				throw InterpreterException("Cannot subtract non-numbers.");
			}
			while(current)
			{
				Number *atom = (Number*)current->eval(env);
				if(atom->type() == NUMBER)
				{
					result->val -= atom->val;
					current = (Number*)current->next;
				}
				else
				{
					throw InterpreterException("Cannot subtract non-numbers.");
				}
			}
		}
		return result;
	}

	Atom* divProc(Env* env, Atom* rest)
	{
		Number *result = new Number(number(0.0));
		Number *current = (Number*)rest;
		if(current)
		{
			Number *atom = (Number*)current->eval(env);
			if(atom->type() == NUMBER)
			{
				result->val = atom->val;
				current = (Number*)current->next;
			}else
			{
				throw InterpreterException("Cannot multiple non-numbers.");
			}
			while(current)
			{
				Number *atom = (Number*)current->eval(env);
				if(atom->type() == NUMBER)
				{
					if(atom->val != number(0.0)) // TODO Caching (YEAH I LOVE POINTLESS OPTIMIZATIONS!).
					{
						result->val /= atom->val;
						current = (Number*)current->next;
					}
					else
					{
						throw InterpreterException("Division by zero");
					}
				}
				else
				{
					throw InterpreterException("Cannot divide non-numbers.");
				}
			}
		}
		return result;
	}

	Atom* printProc(Env* env, Atom* rest)
	{
		//std::cout << "Print proc" << std::endl;
		Atom *current = rest;
		while(current)
		{
			Atom *atom = current->eval(env);
			current = current->next;
			std::cout << atom->repr();
		}
		//std::cout << std::endl;
		return new List();
	}

	Atom* quoteProc(Env* env, Atom* rest)
	{
		//std::cout << "Quote proc" << std::endl;
		return rest ? rest->evalq(env) : new List();
	}

	Atom* defineProc(Env* env, Atom* rest)
	{
		std::cout << "Define proc" << std::endl;
		if(rest)
		{
			if(rest->type() == SYMBOL)
			{
				if(rest->next)
				{
					env->set(rest->repr(), rest->next->eval(env));
				}else
				{
					throw InterpreterException("Define without second argument");
				}
			}else
			{
				throw InterpreterException("Trying to define non-symbol");
			}
		}else
		{
			throw InterpreterException("Empty definition");
		}
		std::cout << "End of define proc" << std::endl;
		return new List();
	}

	Atom* setProc(Env* env, Atom* rest)
	{
		//std::cout << "Set proc" << std::endl;
		if(rest)
		{
			if(rest->type() == SYMBOL)
			{
				if(rest->next)
				{
					env->find(rest->repr())->set(rest->repr(), rest->next->eval(env));
				}else
				{
					throw InterpreterException("Set without second argument");
				}
			}else
			{
				throw InterpreterException("Trying to set non-symbol");
			}
		}else
		{
			throw InterpreterException("Empty set");
		}
		return new List();
	}


	void addBuiltIn(const string &name, BuiltInFunc func, Env *env)
	{
		BuiltIn::add(name, func);

		//tokensVector tokens;
		//tokenize(string("((built-in define) ") + name + string(" (built-in ") + name + string("))"), tokens);

		//Atom *atom = makeAST(tokens, env);
		//atom->evalf(env);
		env->set(name, gc_new BuiltIn(env, name));
	}

	void prepareBuiltIns(Env *env)
	{
		//TODO Macros (fexprs?).
		BuiltIn::add(string("begin"), beginProc); // Must be added first.
		addBuiltIn(string("define"), defineProc, env); // And this must be second.
		addBuiltIn(string("set"), setProc, env);
		addBuiltIn(string("begin"), beginProc, env);
		addBuiltIn(string("quote"), quoteProc, env);
		addBuiltIn(string("+"), addProc, env);
		addBuiltIn(string("-"), subProc, env);
		addBuiltIn(string("*"), mulProc, env);
		addBuiltIn(string("/"), divProc, env);
		addBuiltIn(string("print"), printProc, env);
	}



	class LispScript : public gc_cleanup
	{
	public:
		LispScript(const string &code)
		{
			env = new Env();
			prepareBuiltIns(env);
			parseCode(code);
		}
		/// Parses code, prepares AST etc.
		void parseCode(const string &code)
		{
			tokensVector tokens;
			tokenize(code, tokens);
			ast = makeAST(tokens, env);
		}
		/// Runs code and return result.
		Atom* run()
		{
			return ast->evalf(env);
		}
		/// Gets global definition.
		Atom* getDef(const string &name)
		{
			return env->get(name);
		}
		/// Sets global definition.
		void setDef(const string &name, Atom *val)
		{
			return env->set(name, val);
		}

	private:
		/// Our precious code.
		string code;
		/// AST first node.
		Atom *ast;

		/// Script's environment.
		Env *env;
	};
}


#endif
