#ifndef PIP_STRING_H_
#define PIP_STRING_H_


//TODO Conditional macros based compilation

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

// Pr0 extensibility.
#define STR_LENGTH(s) ((s).length())
#define STR_SUBSTR(s, from, length) ((s).substr((from), (length)))
#define STR_FIND(s, sub, offset) ((s).find((sub), (offset)))
#define STR_REPLACE(s, pos, length, str) ((s).replace((pos), (length), (str)))
#define STR_CCHR(s) ((s).c_str())

namespace pip
{
	typedef std::string string;
	template <class T> inline bool from_string(T& t,const string& s,std::ios_base& (*f)(std::ios_base&)){ std::istringstream iss(s); return !(iss >> f >> t).fail();}

	inline bool loadFile(const string& filename, string& str)
	{
		std::ifstream file;
		file.open(STR_CCHR(filename));
		if(file.good())
		{
			string line;
			while (getline(file, line))
				str += line + '\n';
			file.close();
			return true;
		}else
		{
			file.close();
			return false;
		}
	}

	//Thanks RosettaCode.
	inline int occurrences(const string& str, const string& sub, size_t from = 0, size_t to = -1)
	{
		if (STR_LENGTH(sub) == 0) return 0;
		int count = 0;
		string s = STR_SUBSTR(str, from, (to >= 0 ? to : str.length()) - from );
		for (size_t offset = s.find(sub); offset != string::npos;
				offset = STR_FIND(s, sub, offset + STR_LENGTH(sub)))
		{
			++count;
		}
		return count;
	}

	inline void replace(string& str, const string& from, const string& to, bool obeyQuotes = true) {
		if(STR_LENGTH(from) == 0)
			return;
		size_t start_pos = 0;
		if (obeyQuotes)
		{
			while((start_pos = STR_FIND(str, from, start_pos)) != string::npos)
			{
				if(occurrences(str, "\"", 0, start_pos) % 2 == 0)
					STR_REPLACE(str, start_pos, from.length(), to);
				start_pos += STR_LENGTH(to);
			}
		}
		else
		{
			while((start_pos = STR_FIND(str, from, start_pos)) != string::npos)
			{
				STR_REPLACE(str, start_pos, from.length(), to);
				start_pos += STR_LENGTH(to);
			}
		}
	}

	struct Token
	{
		Token(const string nval, bool nquoted = false) : val(nval), quoted(nquoted) {}
		string val;
		bool quoted;
	};
	typedef std::vector<Token> tokensVector;

	//TODO Out-of-bounds errors.
	inline void tokenize(const string &str, tokensVector &tokens)
	{
		string s = string("(") + str + string(")");
		s.append(" ");
		replace(s, "(", " ( ");
		replace(s, ")", " ) ");

		size_t current = 0;
		while(current != STR_LENGTH(s))
		{
			if (s[current] == '"')
			{
				size_t oldCurrent = current;
				for(current++;s[current] != '"'; current++);
				string token = STR_SUBSTR(s, oldCurrent + 1, current - oldCurrent - 1);
				//std::cout << "Found quoted token '" << token << "'." << std::endl;
				tokens.push_back(Token(token, true));
			}else if(s[current] != ' ')
			{
				size_t oldCurrent = current;
				for(current++;s[current] != ' '; current++);
				string token = STR_SUBSTR(s, oldCurrent, current - oldCurrent);
				//std::cout << "Found token '" << token << "'." << std::endl;
				tokens.push_back(Token(token));
			}
			current++;
		}
		//std::cout << "Tokenized." << std::endl;
	}
}

#endif
