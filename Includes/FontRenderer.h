#pragma once
#include "RenderContext.h"

typedef std::function<void(float xRatio, float yRatio)> OnSizeChangeCallback_TYPE;

struct IFont : public IHandle {
    struct Glyph {
        vector<uint8_t> Data;
        uint32_t Width;
        uint32_t Height;
        uint32_t OriginX;
        uint32_t OriginY;
        uint32_t StepX;
        uint32_t StepY;
    };
    virtual bool        GetGlyph(wchar_t c, Glyph& outGlyph) = 0;
    virtual std::string GetName() const = 0;
    virtual uint32_t    GetSize() const = 0;

    static IFont* Create(const std::string& name, uint32_t size);
};

struct ICompiledString 
    : public IHandle
    , public IRenderable {
    virtual void            SetPosition(uint32_t x, uint32_t y) = 0;
    virtual std::wstring    GetText() const = 0;
};

struct IFontAtlas {
    virtual void SetFont(IFont* font) = 0;
    virtual void RegisterOnSizeChangeCallback(const OnSizeChangeCallback_TYPE& f) = 0;
};

struct ICompiledStringBuilder : public IHandle {
    virtual void                SetFont(const IFont* font) = 0;
    virtual void                SetColor(const Vector3<float>& color) = 0;
    virtual ICompiledString*    CompileString(const std::wstring& text) = 0;

    static IFontRenderer*       Create(IRenderContext* renderContext);
};
