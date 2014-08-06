#include "SceneImpl.h"
#include "RenderContext.h"
#include "ErrorHandler.h"
#include "ObjectImpl.h"
#include "CameraImpl.h"
#include "LightImpl.h"

static const unsigned char g_vertexShaderSource[] = {
    #include "vshader.shader.h"
    0x00,
};

static const unsigned char g_fragmentShader[] = {
    #include "fshader.shader.h"
    0x00,
};

Scene::Scene()
    : m_camera(nullptr)
    , m_vertexShaderSource((char*)g_vertexShaderSource)
    , m_fragmentShaderSource((char*)g_fragmentShader)
{
    initShaders();
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glUseProgram(m_program.Program));
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearDepthf(1.0f);
}

Scene::~Scene()
{
    if (m_program.Program)
    {
        glDeleteProgram(m_program.Program);
        m_program.Program = 0;
    }
}

void Scene::AddObject(IObjectPtr& object)
{
    m_objects.insert(static_pointer_cast<Object>(object));
}

void Scene::AddLight(ILightPtr& light)
{
    m_lights.insert(static_pointer_cast<Light>(light));
}

void Scene::SetCamera(ICameraPtr& camera)
{
    m_camera = static_pointer_cast<Camera>(camera);
}

void Scene::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set pipline states
	GL_CALL(glUniformMatrix4fv(m_program.UniformViewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetViewMatrix())));
    GL_CALL(glUniformMatrix4fv(m_program.UniformProjMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjectionMatrix())));
    if (!m_lights.empty())
    {
        ILightPtr light = *m_lights.begin();
        GL_CALL(glUniform3fv(m_program.UniformLightPosition, 1, glm::value_ptr(light->GetPosition())));
    }
	// Set texture
	//GL_CALL(glActiveTexture(GL_TEXTURE0));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, g_Texture));
	//GL_CALL(glUniform1i(g_UniformTexture0, 0));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    GL_CALL(glUseProgram(m_program.Program));
    GL_CALL(glEnableVertexAttribArray(m_program.AttribPosition));
	// GL_CALL(glEnableVertexAttribArray(g_AtribTexCoord));

	// Draw
    for (auto object : m_objects)
    {
        // Get object desc
        const Object::GLMeshDescs& objectGLDescs = object->GetMeshDescs();
        const IMesh::Desc& meshDesc = object->GetMesh()->GetDesc();
        GL_CALL(glUniformMatrix4fv(m_program.UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(object->GetMatrix(CoordType::Local))));
        GL_CALL(glUniformMatrix4fv(m_program.UniformWorldMatrix, 1, GL_FALSE, glm::value_ptr(object->GetMatrix(CoordType::World))));

	    // glActiveTexture(GL_TEXTURE0);
	    // GL_CALL(glBindTexture(GL_TEXTURE_2D, g_Texture));
        for (uint32_t i = 0; i < objectGLDescs.size(); ++i)
        {
            auto& objectDesc = objectGLDescs[i];
            auto& shape = meshDesc.Shapes[i];
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, objectDesc.VertexBuffer));
            GL_CALL(glVertexAttribPointer(m_program.AttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0));

            if (objectDesc.NormalBuffer)
            {
                GL_CALL(glEnableVertexAttribArray(m_program.AttribNormal));
                GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, objectDesc.NormalBuffer));
                GL_CALL(glVertexAttribPointer(m_program.AttribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0));
            }
            if (objectDesc.IndexBuffer)
            {
                uint32_t numVertices = shape.Indices.Data.size() / shape.Indices.ElementSize;
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectDesc.IndexBuffer));
                GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0));
            }
            else
            {
                uint32_t numVertices = shape.Positions.Data.size() / shape.Positions.ElementSize;
                GL_CALL(glDrawArrays(GL_TRIANGLES, 0, numVertices));
            }
	        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }
    }
	// Disable shader variables
	GL_CALL(glDisableVertexAttribArray(m_program.AttribPosition));
	//GL_CALL(glDisableVertexAttribArray(g_AtribTexCoord));
}

void Scene::initShaders()
{
    if (m_program.Valid)
        return;
    if (m_program.Program)
    {
        // Delete old program
        glDeleteProgram(m_program.Program);
        m_program.Program = 0;
    }
    // Shader creation
    GLuint vertexShader = compileShader(m_vertexShaderSource, GL_VERTEX_SHADER);
    VE_ERROR_IF(!vertexShader, L"Failed to create vertex shader with source: %s", m_vertexShaderSource.c_str());
    GLuint fragmentShader = compileShader(m_fragmentShaderSource, GL_FRAGMENT_SHADER);
    VE_ERROR_IF(!fragmentShader, L"Failed to create fragment shader with source: %s", m_fragmentShaderSource.c_str());

    m_program.Program = GL_CALL(glCreateProgram());
    GL_CALL(glAttachShader(m_program.Program, vertexShader));
    GL_CALL(glAttachShader(m_program.Program, fragmentShader));
    GL_CALL(glLinkProgram(m_program.Program));
    GLint result = 0;
    GL_CALL(glGetProgramiv(m_program.Program, GL_LINK_STATUS, &result));
    if (!result)
    {
        GLchar errorLog[1024];
        GL_CALL(glGetProgramInfoLog(m_program.Program, 1024, NULL, errorLog));
        VE_ERROR(L"Failed to link program: %s", errorLog);
    }
    
    GL_CALL(glValidateProgram(m_program.Program));
    GL_CALL(glGetProgramiv(m_program.Program, GL_VALIDATE_STATUS, &result));
    if (!result)
    {
        GLchar errorLog[1024];
        GL_CALL(glGetProgramInfoLog(m_program.Program, 1024, NULL, errorLog));
        VE_ERROR(L"Failed to validate program: %s", errorLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_program.AttribPosition = GL_CALL(glGetAttribLocation(m_program.Program, "position"));
    VE_ERROR_IF(m_program.AttribPosition == -1, L"Failed to get location of attribute \"position\"");

    m_program.AttribNormal = GL_CALL(glGetAttribLocation(m_program.Program, "normal"));
    VE_ERROR_IF(m_program.AttribNormal == -1, L"Failed to get location of attribute \"normal\"");

    m_program.UniformModelMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "modelMatrix"));
    VE_ERROR_IF(m_program.UniformModelMatrix == -1, L"Failed to get location of attribute \"modelMatrix\"");

    m_program.UniformWorldMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "worldMatrix"));
    VE_ERROR_IF(m_program.UniformWorldMatrix == -1, L"Failed to get location of attribute \"worldMatrix\"");

    m_program.UniformViewMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "viewMatrix"));
    VE_ERROR_IF(m_program.UniformViewMatrix == -1, L"Failed to get location of attribute \"viewMatrix\"");

    m_program.UniformProjMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "projMatrix"));
    VE_ERROR_IF(m_program.UniformProjMatrix == -1, L"Failed to get location of attribute \"projMatrix\"");
    
    m_program.UniformLightPosition = GL_CALL(glGetUniformLocation(m_program.Program, "lightPosition"));
    VE_ERROR_IF(m_program.UniformLightPosition == -1, L"Failed to get location of attribute \"lightPosition\"");
    //g_AtribTexCoord = GL_CALL(glGetAttribLocation(g_Program, "textureCoord"));
    //if (g_AtribTexCoord == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"textureCoord\"");

    //g_UniformTexture0 = GL_CALL(glGetUniformLocation(g_Program, "texture0"));
    //if (g_UniformTexture0 == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"texture0\"");

    m_program.Valid = true;
}

GLuint Scene::compileShader(const std::string source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const char* s = source.c_str();
    GL_CALL(glShaderSource(shader, 1, &s, 0));
    GL_CALL(glCompileShader(shader));

    GLint result = 0;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        GLchar errorLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, errorLog);
        VE_ERROR(L"Failed to compile shader: %S", errorLog);
    }

    return shader;
}

