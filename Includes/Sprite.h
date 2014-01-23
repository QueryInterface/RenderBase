#pragma once
#include "RenderContext.h"
#include "MathBasics.h"

struct ITexture2D;

struct ISprite 
    : public IHandle 
    , public IRenderable {
    virtual void 			SetPosition(uint32_t x, uint32_t y) = 0;
    virtual void 			SetSize(uint32_t width, uint32_t height) = 0;
    virtual void 			SetColor(const Vector3<float>& color) = 0;
    virtual void 			SetOpacity(float a) = 0;
    virtual void 			SetTexture(ITexture2D* texture) = 0;
    virtual void 			SetTextureCoords(const Rect<float>& rect) = 0;

    virtual uint32_t 		GetX() const = 0;
    virtual uint32_t 		GetY() const = 0;
    virtual uint32_t 		GetWidth() const = 0;
    virtual uint32_t 		GetHeight() const = 0;
    virtual Vector3<float> 	GetColor() const = 0;
    virtual float 			GetOpacity() const = 0;
    virtual ITexture2D*	    GetTexture() const = 0;
    virtual Rect<float>	    GetTextureCoords() const = 0;

    static ISprite* Create(IRenderContext* renderContext);
};