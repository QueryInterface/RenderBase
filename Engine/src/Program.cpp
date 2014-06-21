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
    , m_compiled(false)
{
}

Program::~Program()
{
}

void Program::SetVertexShader(const std::string& vertex)
{
    m_vertexShaderSource = vertex;
    m_compiled = false;
}

void Program::SetFragmentShader(const std::string& fragment)
{
    m_fragmentShaderSource = fragment;
    m_compiled = false;
}

int Program::GetAttribLocation(const std::string& name)
{
    if (!m_compiled) return -1;
    int ret = GL_CALL(glGetAttribLocation(m_program, name.c_str()));
    VE_WARNING_IF(ret == -1, L"Failed to get attribute location for %s", name.c_str());
    return ret;
}

int Program::GetUniformLocation(const std::string& name)
{
    if (!m_compiled) return -1;
    int ret = GL_CALL(glGetUniformLocation(m_program, name.c_str()));
    VE_WARNING_IF(ret == -1, L"Failed to get uniform location for %s", name.c_str());
    return ret;
}

GLuint Program::GetGLProgram() const
{
    return m_program;
}

void Program::Compile()
{
    if (m_compiled) return;
	// Shader creation
	m_vertexShader = compileShader(GL_VERTEX_SHADER);
    VE_ERROR_IF(!m_vertexShader, L"Failed to create vertex shader with source: %s", m_vertexShaderSource.c_str());
	m_fragmentShader = compileShader(GL_FRAGMENT_SHADER);
    VE_ERROR_IF(!m_fragmentShader, L"Failed to create fragment shader with source: %s", m_fragmentShaderSource.c_str());

	m_program = GL_CALL(glCreateProgram());
	GL_CALL(glAttachShader(m_program, m_vertexShader));
	GL_CALL(glAttachShader(m_program, m_fragmentShader));
	GL_CALL(glLinkProgram(m_program));
	GLint result = 0;
	GL_CALL(glGetProgramiv(m_program, GL_LINK_STATUS, &result));
	if (!result)
	{
		GLchar errorLog[1024];
		GL_CALL(glGetProgramInfoLog(m_program, 1024, NULL, errorLog));
		VE_ERROR(L"Failed to link program: %s", errorLog);
	}

	GL_CALL(glValidateProgram(m_program));
	GL_CALL(glGetProgramiv(m_program, GL_VALIDATE_STATUS, &result));
	if (!result)
	{
		GLchar errorLog[1024];
		GL_CALL(glGetProgramInfoLog(m_program, 1024, NULL, errorLog));
		VE_ERROR(L"Failed to validate program: %s", errorLog);
	}
    m_compiled = true;
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
