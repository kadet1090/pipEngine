#include "Shader.h"

int pip::Shader::currentTextureUnit = 0;
map<pip::string,GLhandleARB> pip::Shader::vertexShadersMap;
map<pip::string,GLhandleARB> pip::Shader::fragmentShadersMap;
map<pip::string,int> pip::Shader::samplersToUnitMap;
map<pip::string,int> pip::Shader::samplersToLocationsMap;

void pip::Shader::loadShader(const string &vertexName, const string &fragmentName)
{
	//printLog(this->prog);

	// Loading vertex shader.
	if(this->vertexShadersMap.count(vertexName) == 0) // TODO Error checking?
	{
		this->vertexShadersMap[vertexName] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		//printLog(this->vertexShadersMap[vertexName]);

		string program;
		if(loadFile(vertexName, program))
		{
			const char * vv = STR_CCHR(program);
			std::cout << "Program: " << vv << std::endl;
			glShaderSourceARB(this->vertexShadersMap[vertexName], 1, &vv, NULL);

			/*GLenum glErr;

			    glErr = glGetError();
			    while (glErr != GL_NO_ERROR)
			    {
			        printf("%s\n", gluErrorString(glErr));
			        glErr = glGetError();
			    }*/
			//delete vv;
			glCompileShaderARB(this->vertexShadersMap[vertexName]);

			//printLog(this->vertexShadersMap[vertexName]);
			GLint result = GL_FALSE;
			glGetObjectParameterivARB(this->vertexShadersMap[vertexName],GL_COMPILE_STATUS,&result);
			if(result == GL_FALSE)
			{
				//printLog(this->vertexShadersMap[vertexName]);
				throw EngineException("Error occurred when compiling '" + vertexName + "' vertex shader.");
			}
		}else
			throw EngineException("Can't load '" + vertexName + "' shader!");
	}

	// Loading fragment shader.
	if(this->fragmentShadersMap.count(fragmentName) == 0)
	{
		this->fragmentShadersMap[fragmentName] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		string program;
		if(loadFile(fragmentName, program))
		{
			const char * ff = STR_CCHR(program);
			glShaderSourceARB(this->fragmentShadersMap[fragmentName], 1, &ff,NULL);
			//delete ff;
			glCompileShaderARB(this->fragmentShadersMap[fragmentName]);
			GLint result = GL_FALSE;
			glGetObjectParameterivARB(this->fragmentShadersMap[fragmentName],GL_COMPILE_STATUS,&result);
			if(result == GL_FALSE)
				throw EngineException("Error occurred when compiling '" + fragmentName + "' fragment shader.");

		}else
			throw EngineException("Can't load '" + fragmentName + "' shader!");
	}

	this->prog = glCreateProgramObjectARB();

	// Attaching vertex shader to program object.
	glAttachObjectARB(this->prog, this->vertexShadersMap[vertexName]);

	// Attaching fragment shader to program object.
	glAttachObjectARB(this->prog, this->fragmentShadersMap[fragmentName]);

	glLinkProgramARB(this->prog);

	GLint result = GL_FALSE;
	glGetObjectParameterivARB(this->prog,GL_OBJECT_LINK_STATUS_ARB,&result);
	if(result == GL_FALSE)
		throw EngineException("Error occurred when linking shaders.");
}

void pip::Shader::setSampler(const string &uniformName, Texture *texture)
{
	if(this->samplersToUnitMap.count(uniformName) == 0)
	{
		this->samplersToUnitMap[uniformName] = currentTextureUnit;
		currentTextureUnit++;
	}
	if(this->samplersToLocationsMap.count(uniformName) == 0)
	{
		GLint tex_sampler = glGetUniformLocationARB(this->prog, uniformName.c_str());
		std::cout << "location: " <<tex_sampler << std::endl;

		string err;
		getOGLErrors(err);
		if(!err.empty())
			throw EngineException("Errors while getting texture sampler:\n" + err);
		this->samplersToLocationsMap[uniformName] = tex_sampler;
	}


	glActiveTexture(GL_TEXTURE0 + this->samplersToUnitMap[uniformName]); // Activate texture unit.
	texture->bind();
	glUniform1iARB(this->samplersToLocationsMap[uniformName], this->samplersToUnitMap[uniformName]); // TODO But what, if referring to inexistent texture unit?
	glActiveTexture(GL_TEXTURE0);
	string err;
	getOGLErrors(err);
	if(!err.empty())
		throw EngineException("Errors while setting texture sampler:\n" + err);
}

void pip::Shader::bind() const
{
	glUseProgramObjectARB(this->prog);
}

void pip::Shader::unbind() const
{
	//TODO Hmm, I'm certain, that it's error in >OpenGL 3.0. But who cares?
	glUseProgramObjectARB(0);
}