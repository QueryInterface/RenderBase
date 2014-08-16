#pragma once
#include "Engine.h"
#include <map>
#include <set>
#include "SDL_opengles2.h"

class Object;
class Camera;
class Light;

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<Light>  LightPtr;

class Scene : public IScene
{
public:
    Scene();
    ~Scene();

    virtual void AddObject(IObjectPtr& object) override;
    virtual void AddLight(ILightPtr& light) override;
    virtual void SetAmbientLight(const vector3f_t& light) override;
    virtual void SetCamera(ICameraPtr& camera) override;

    virtual void Render();
private:
    struct ProgramDesc
    {
        ProgramDesc() 
            : Program(0)
            , AttribPosition(0)
            , AttribNormal(0)
            , UniformModelViewMatrix(0)
            , UniformNormalMatrix(0)
            , UniformViewMatrix(0)
            , UniformProjMatrix(0)
            , UniformLightPosition(0)
            , Valid(false) {}

        GLuint      Program;
        GLint       AttribPosition;
        GLint       AttribNormal;
        GLint       UniformModelViewMatrix;
        GLint       UniformNormalMatrix;
        GLint       UniformViewMatrix;
        GLint       UniformProjMatrix;
        GLint       UniformLightPosition;
        GLint       UniformAmbientLight;
        bool        Valid;
    };

    std::set<ObjectPtr>                     m_objects;
    std::set<LightPtr>                      m_lights;
    vector3f_t                              m_ambientLight;
    CameraPtr                               m_camera;

    ProgramDesc                             m_program;
    std::string                             m_vertexShaderSource;
    std::string                             m_fragmentShaderSource;
    
	void initShaders();

    void compileProgram(const std::string& vertex, const std::string fragment);
    GLuint compileShader(const std::string source, GLenum type);

};