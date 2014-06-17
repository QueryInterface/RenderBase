#pragma once
#include "Engine.h"
#include "SDL_opengles2.h"

class Program
{
public:
    Program();
    Program(const std::string& vertex, const std::string fragment);
    virtual ~Program();
    
    virtual void SetVertexShader(const std::string& vertex);
    virtual void SetFragmentShader(const std::string& fragment);

    virtual GLuint GetGLProgram() const;

    virtual void Compile();
private:
    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;
    GLuint      m_vertexShader;
    GLuint	    m_fragmentShader;
    GLuint	    m_program;

    GLuint compileShader(GLenum shaderType);
};