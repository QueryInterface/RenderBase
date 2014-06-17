#include "ErrorHandler.h"
#include "Program.h"

Program::Program()
    : m_vertexShaderSource("")
    , m_fragmentShaderSource("")
    , m_vertexShader(GLuint(-1))
    , m_fragmentShader(GLuint(-1))
    , m_program(GLuint(-1))
{
}

Program::Program(const std::string& vertex, const std::string fragment)
    : m_vertexShaderSource(vertex)
    , m_fragmentShaderSource(fragment)
    , m_vertexShader(GLuint(-1))
    , m_fragmentShader(GLuint(-1))
    , m_program(GLuint(-1))
    
{
}

Program::~Program()
{
}

void Program::SetVertexShader(const std::string& vertex)
{
    m_vertexShaderSource = vertex;
}

void Program::SetFragmentShader(const std::string& fragment)
{
    m_fragmentShaderSource = fragment;
}

GLuint Program::GetGLProgram() const
{
    return m_program;
}

void Program::Compile()
{

}

GLuint Program::compileShader(GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	const char* shaderSource = nullptr;
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        {
            shaderSource = m_vertexShaderSource.c_str();
        }
        break;
    case GL_FRAGMENT_SHADER:
        {
            shaderSource = m_fragmentShaderSource.c_str();
        }
        break;
    default:
        VE_ERROR(L"Invalid shader type: %d", shaderType);
    }
	GL_CALL(glShaderSource(shader, 1, &shaderSource, 0));
	GL_CALL(glCompileShader(shader));
	
	GLint result = 0;
	GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (!result)
	{
		GLchar errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		VE_ERROR(L"Failed to compile shader: %s", errorLog);
	}

	return shader;
}
