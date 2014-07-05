#include "SceneImpl.h"
#include "RenderContext.h"
#include "ErrorHandler.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

const std::string g_vertexShaderSource =                \
"   #version 120                                        \
                                                        \
    attribute vec3 position;                            \
    attribute vec2 textureCoord;                        \
                                                        \
    uniform mat4 modelMatrix;                           \
    uniform mat4 worldMatrix;                           \
    uniform mat4 viewMatrix;                            \
    uniform mat4 projMatrix;                            \
                                                        \
    varying vec2 f_textureCoord;                        \
                                                        \
    void main(void)                                     \
    {                                                   \
        gl_Position = modelMatrix * vec4(position, 1.0);\
        gl_Position = worldMatrix * gl_Position;        \
        gl_Position = viewMatrix * gl_Position;         \
        gl_Position = projMatrix * gl_Position;         \
        f_textureCoord = textureCoord;                  \
    }";

const std::string g_fragmentShader =                        \
"   #version 120                                            \
                                                            \
    void main(void) {                                       \
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);            \
    }";



Scene::Scene()
    : m_camera(nullptr)
    , m_vertexShaderSource(g_vertexShaderSource)
    , m_fragmentShaderSource(g_fragmentShader)
{
}

Scene::~Scene()
{
    if (m_program.Program)
    {
        glDeleteProgram(m_program.Program);
        m_program.Program = 0;
    }
    for (auto object : m_objects)
    {
        // Get object desc
        object_descs_t& objectDescs = m_objectDescs[object];
        // Delete old buffers
        for (auto& desc : objectDescs)
        {
            if (desc.VertexBuffer)
            {
                glDeleteBuffers(1, &desc.VertexBuffer);
                desc.VertexBuffer = 0;
            }
            if (desc.IndexBuffer)
            {
                glDeleteBuffers(1, &desc.IndexBuffer);
                desc.IndexBuffer = 0;
            }
        }
    }
}

void Scene::AddObject(IObjectPtr& object)
{
    m_objects.insert(object);
}

void Scene::AddLight(ILightPtr& light)
{
    m_lights.insert(light);
}

void Scene::SetCamera(ICameraPtr& camera)
{
    m_camera = camera;
}

void Scene::Render()
{
    initShaders();
    initObjectsData();
    initPipeline();

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	GL_CALL(glUseProgram(m_program.Program));
	GL_CALL(glEnableVertexAttribArray(m_program.AttribPosition));
	// GL_CALL(glEnableVertexAttribArray(g_AtribTexCoord));

	// Draw
    for (auto object : m_objects)
    {
        // Get object desc
        object_descs_t& objectDesc = m_objectDescs[object];
        IMeshPtr mesh = nullptr;
        const IMesh::Desc* meshDesc = mesh->GetDesc();
	    // glActiveTexture(GL_TEXTURE0);
	    // GL_CALL(glBindTexture(GL_TEXTURE_2D, g_Texture));
        for (uint32_t i = 0; i < objectDesc.size(); ++i)
        {
            auto& shape = meshDesc->Shapes[i];
            uint32_t numTriangles = shape.Positions.Data.size() / shape.Positions.ElementSize;
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, objectDesc[i].VertexBuffer));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectDesc[i].IndexBuffer));
            GL_CALL(glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_SHORT, 0));
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

    m_program.UniformModelMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "modelMatrix"));
    VE_ERROR_IF(m_program.UniformModelMatrix == -1, L"Failed to get location of attribute \"modelMatrix\"");

    m_program.UniformWorldMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "worldMatrix"));
    VE_ERROR_IF(m_program.UniformWorldMatrix == -1, L"Failed to get location of attribute \"worldMatrix\"");

    m_program.UniformViewMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "viewMatrix"));
    VE_ERROR_IF(m_program.UniformViewMatrix == -1, L"Failed to get location of attribute \"viewMatrix\"");

    m_program.UniformProjMatrix = GL_CALL(glGetUniformLocation(m_program.Program, "projMatrix"));
    VE_ERROR_IF(m_program.UniformProjMatrix == -1, L"Failed to get location of attribute \"projMatrix\"");

    //g_AtribTexCoord = GL_CALL(glGetAttribLocation(g_Program, "textureCoord"));
    //if (g_AtribTexCoord == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"textureCoord\"");

    //g_UniformTexture0 = GL_CALL(glGetUniformLocation(g_Program, "texture0"));
    //if (g_UniformTexture0 == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"texture0\"");

    m_program.Valid = true;
}

void Scene::initObjectsData()
{
    for (auto object : m_objects)
    {
        // Get object desc
        object_descs_t objectDescs = m_objectDescs[object];
        for (uint32_t i = 0; i < objectDescs.size(); ++i)
        {
            if (objectDescs[i].Valid)
                continue;

            // Delete old buffers
            if (objectDescs[i].VertexBuffer)
            {
                glDeleteBuffers(1, &objectDescs[i].VertexBuffer);
                objectDescs[i].VertexBuffer = 0;
            }
            if (objectDescs[i].IndexBuffer)
            {
                glDeleteBuffers(1, &objectDescs[i].IndexBuffer);
                objectDescs[i].IndexBuffer = 0;
            }
            // Init new data
            IMesh::GeometryDesc meshDesc;
            IMeshPtr mesh = nullptr;
            mesh->GetGeometryDesc(meshDesc);
            for (auto layoutItem : meshDesc.layout)
            {
                switch (layoutItem.layoutType)
                {
                case IMesh::LayoutType::Vertices:
                    {
                        GL_CALL(glGenBuffers(1, &objectDesc.VertexBuffer));
                        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, objectDesc.VertexBuffer));
                        GL_CALL(glBufferData(GL_ARRAY_BUFFER, layoutItem.itemsCount * sizeof(float), layoutItem.items, GL_STATIC_DRAW));
                        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
                    }
                    break;
                default:
                    break;
                }
            }
            // Init vertex indices
            if (meshDesc.groups.size() && meshDesc.groups[0].indices)
            {
                GL_CALL(glGenBuffers(1, &objectDesc.IndexBuffer));
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectDesc.IndexBuffer));
                GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshDesc.groups[0].count * sizeof(uint32_t), meshDesc.groups[0].indices, GL_STATIC_DRAW));
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            }
            objectDesc.Valid = true;
        }
    }
    //// Geometry creation
    //// Bind vertex positions
    //GL_CALL(glGenBuffers(1, &g_VBOVertices));
    //GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, g_VBOVertices));
    //GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(g_pCubeVertices), g_pCubeVertices, GL_STATIC_DRAW));
    //GL_CALL(glVertexAttribPointer(g_AtribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0));
    //GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    //// Bind vertex tex coodinates
    //glGenBuffers(1, &g_VBOTexCoords);
    //glBindBuffer(GL_ARRAY_BUFFER, g_VBOTexCoords);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_pTexCoord), g_pTexCoord, GL_STATIC_DRAW);
    //glVertexAttribPointer(g_AtribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene::initPipeline()
{
	// Set matrices
    IWindow* window = IEngine::Instance()->GetWindow();
    float aspect = 1.0f * window->GetWidth() / window->GetHeight();
	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::mat4 worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -7.0));
	//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 10.0f);

	// Set pipline states
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glUseProgram(m_program.Program));
    GL_CALL(glUniformMatrix4fv(m_program.UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix)));
    GL_CALL(glUniformMatrix4fv(m_program.UniformWorldMatrix, 1, GL_FALSE, glm::value_ptr(worldMatrix)));
	GL_CALL(glUniformMatrix4fv(m_program.UniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix)));
    GL_CALL(glUniformMatrix4fv(m_program.UniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
	// Set texture
	//GL_CALL(glActiveTexture(GL_TEXTURE0));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, g_Texture));
	//GL_CALL(glUniform1i(g_UniformTexture0, 0));
	//GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	GL_CALL(glUseProgram(0));
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
        VE_ERROR(L"Failed to compile shader: %s", errorLog);
    }

    return shader;
}

