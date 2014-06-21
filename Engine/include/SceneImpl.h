#pragma once
#include "Engine.h"
#include "Program.h"
#include <map>

class Scene : public IScene
{
public:
    Scene();
    ~Scene();

    virtual void AddObject(IObjectPtr& object) override;
    virtual void AddLight(ILightPtr& light) override;
    virtual void SetCamera(ICameraPtr& camera) override;

    virtual void Render() const override;
private:
    struct ObjectGLDesc
    {
        GLuint vertexId;
        GLuint textureId;
        GLuint indexId;
    };

    std::map<IObjectPtr, ObjectGLDesc>  m_objects;
    std::map<ILightPtr, ObjectGLDesc>   m_lights;
    ICameraPtr                          m_camera;
    Program                             m_program;

	void initShaders();
	void initGeometry();
	void initTextures();
	void initPipeline();
};