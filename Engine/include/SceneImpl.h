#pragma once
#include "Engine.h"
#include "ObjectImpl.h"
#include <map>
#include <set>
#include "SDL_opengles2.h"

class Scene : public IScene
{
public:
    Scene();
    ~Scene();

    virtual void AddObject(IObjectPtr& object) override;
    virtual void AddLight(ILightPtr& light) override;
    virtual void SetCamera(ICameraPtr& camera) override;

    virtual void Render() override;
private:
    struct ProgramDesc
    {
        ProgramDesc() 
            : Program(0)
            , AttribPosition(0)
            , UniformModelMatrix(0)
            , UniformWorldMatrix(0)
            , UniformViewMatrix(0)
            , UniformProjMatrix(0)
            , Valid(false) {}

        GLuint      Program;
        GLint       AttribPosition;
        GLint       UniformModelMatrix;
        GLint       UniformWorldMatrix;
        GLint       UniformViewMatrix;
        GLint       UniformProjMatrix;
        bool        Valid;
    };

    std::set<ObjectPtr>                     m_objects;
    std::set<ILightPtr>                     m_lights;
    ICameraPtr                              m_camera;

    ProgramDesc                             m_program;
    std::string                             m_vertexShaderSource;
    std::string                             m_fragmentShaderSource;
    
	void initShaders();
	void initPipeline();

    void compileProgram(const std::string& vertex, const std::string fragment);
    GLuint compileShader(const std::string source, GLenum type);

};