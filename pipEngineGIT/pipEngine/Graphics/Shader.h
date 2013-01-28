#ifndef SHADER_H_
#define SHADER_H_



//#include "../Default.h"
//#define GC X11_GC
//#include "GLee.h"
//#undef GC
#include "Renderer.h"
#include "Texture.h"

#include <iostream>

namespace pip
{
    /*inline void printShaderLog(GLhandleARB obj)
    {

        glGetObjectParameterivARB(obj,
                                 GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
        pInfoLog = (GLcharARB *) malloc(maxLength * sizeof(GLcharARB));
        glGetInfoLogARB(VertexShaderObject, maxLength, &length, pInfoLog);

        printf("%s\n", pInfoLog);
        free(pInfoLog);
    }*/

    // Wonderful Shader class.
    class Shader : public GCed
    {
    public:
    	void loadShader(const string &vertexName, const string &fragmentName);
    	// Sets sampler2D uniform. Remember, that shader must be binded before calling!
    	void setSampler(const string &uniformName, Texture *texture);
    	void bind() const;
    	void unbind() const;
    private:
    	static int currentTextureUnit;
    	// OpenGL's program object.
    	GLhandleARB prog;
    	// Some managers.
		static map<string,GLhandleARB> vertexShadersMap;
		static map<string,GLhandleARB> fragmentShadersMap;
		// Self-explanatory.
		static map<string,GLint> samplersToUnitMap;
		static map<string,GLint> samplersToLocationsMap;
   };


}

#endif