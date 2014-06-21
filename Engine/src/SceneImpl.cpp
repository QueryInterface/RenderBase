#include "SceneImpl.h"
#include "RenderContext.h"
#include "glm/glm.hpp"

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
    uniform sampler2D texture0;                             \
    varying vec2 f_textureCoord;                            \
                                                            \
    void main(void) {                                       \
	    gl_FragColor = texture2D(texture0, f_textureCoord); \
    }";



Scene::Scene()
    : m_camera(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::AddObject(IObjectPtr& /*object*/)
{
    //m_objects.push_back(object);
}

void Scene::AddLight(ILightPtr& /*light*/)
{
    //m_lights.push_back(light);
}

void Scene::SetCamera(ICameraPtr& camera)
{
    m_camera = camera;
}

void Scene::Render() const
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Scene::initShaders()
{
    m_program.SetVertexShader(g_vertexShaderSource);
    m_program.SetFragmentShader(g_fragmentShader);
    m_program.Compile();
	//// Init shader variables
	//g_AtribPosition = GL_CALL(glGetAttribLocation(g_Program, "position"));
	//if (g_AtribPosition == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"position\"");

	//g_AtribTexCoord = GL_CALL(glGetAttribLocation(g_Program, "textureCoord"));
	//if (g_AtribTexCoord == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"textureCoord\"");

	//g_UniformModelMatrix = GL_CALL(glGetUniformLocation(g_Program, "modelMatrix"));
	//if (g_UniformModelMatrix == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"modelMatrix\"");

	//g_UniformWorldMatrix = GL_CALL(glGetUniformLocation(g_Program, "worldMatrix"));
	//if (g_UniformWorldMatrix == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"worldMatrix\"");

	//g_UniformViewMatrix = GL_CALL(glGetUniformLocation(g_Program, "viewMatrix"));
	//if (g_UniformViewMatrix == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"viewMatrix\"");

	//g_UniformProjMatrix = GL_CALL(glGetUniformLocation(g_Program, "projMatrix"));
	//if (g_UniformProjMatrix == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"projMatrix\"");

	//g_UniformTexture0 = GL_CALL(glGetUniformLocation(g_Program, "texture0"));
	//if (g_UniformTexture0 == -1) LOG_ERROR("main", "Init", "Failed to get location of attribute \"texture0\"");
}

void Scene::initGeometry()
{
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

	//GL_CALL(glGenBuffers(1, &g_IBOElements));
	//GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBOElements));
	//GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_pElements), g_pElements, GL_STATIC_DRAW));
	//GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}