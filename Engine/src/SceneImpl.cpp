#include "SceneImpl.h"
#include "RenderContext.h"
#include "glm/glm.hpp"

Scene::Scene()
    : m_camera(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::AddObject(IObjectPtr& object)
{
    m_objects.push_back(object);
}

void Scene::AddLight(ILightPtr& light)
{
    m_lights.push_back(light);
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
	//// Shader creation
	//g_VertexShader = CompileShader("vs.glsl", GL_VERTEX_SHADER);
	//if (!g_VertexShader) LOG_ERROR("main", "Init", "Failed to create vertex shader");
	//g_PixelShader = CompileShader("ps.glsl", GL_FRAGMENT_SHADER);
	//if (!g_PixelShader) LOG_ERROR("main", "Init", "Failed to create pixel shader");
	//
	//g_Program = GL_CALL(glCreateProgram());
	//GL_CALL(glAttachShader(g_Program, g_VertexShader));
	//GL_CALL(glAttachShader(g_Program, g_PixelShader));
	//GL_CALL(glLinkProgram(g_Program));
	//GLint result = 0;
	//GL_CALL(glGetProgramiv(g_Program, GL_LINK_STATUS, &result));
	//if (!result)
	//{
	//	GLchar errorLog[1024];
	//	GL_CALL(glGetProgramInfoLog(g_Program, 1024, NULL, errorLog));
	//	LOG_ERROR("main", "Init", "Failed to link program: %s", errorLog);
	//}

	//GL_CALL(glValidateProgram(g_Program));
	//GL_CALL(glGetProgramiv(g_Program, GL_VALIDATE_STATUS, &result));
	//if (!result)
	//{
	//	GLchar errorLog[1024];
	//	GL_CALL(glGetProgramInfoLog(g_Program, 1024, NULL, errorLog));
	//	LOG_ERROR("main", "Init", "Failed to validate program: %s", errorLog);
	//}

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