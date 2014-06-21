#pragma once
#include "Engine.h"
#include "SDL_opengles2.h"
#include "glm/glm.hpp"

class Program
{
public:
    Program();
    Program(const std::string& vertex, const std::string fragment);
    virtual ~Program();
    
    virtual void SetVertexShader(const std::string& vertex);
    virtual void SetFragmentShader(const std::string& fragment);
    virtual int  GetAttribLocation(const std::string& name);
    virtual int  GetUniformLocation(const std::string& name);

    virtual GLuint GetGLProgram() const;

    virtual void Compile();
private:
    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;
    GLuint      m_vertexShader;
    GLuint	    m_fragmentShader;
    GLuint	    m_program;
    bool        m_compiled;

    GLuint compileShader(GLenum shaderType);
};