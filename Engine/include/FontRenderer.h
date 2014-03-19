#pragma once
#include "RenderContext.h"
#include "MathBasics.h"
#include <vector>

struct IFontDecl : public IHandle {
    struct Glyph {
        Glyph() { Width = Height = OriginX = OriginY = StepX = StepY = 0; }
        std::vector<uint8_t>    Data;
        uint32_t                Width;
        uint32_t                Height;
        int32_t                 OriginX;
        int32_t                 OriginY;
        int32_t                 StepX;
        int32_t                 StepY;

        IFontDecl*              Font;
        wchar_t                 Symbol;
    };
    virtual bool        GetGlyph(wchar_t c, Glyph& outGlyph) = 0;
    virtual std::string GetName() const = 0;
    virtual uint32_t    GetLineSpace() const = 0;
    virtual uint32_t    GetSize() const = 0;
    virtual uint32_t    GetKerning(wchar_t c0, wchar_t c1) const = 0;

    static LIB_EXPORT IFontDecl* CALLING_CONVENTION Create(const std::string& name, uint32_t size);
};

struct ICompiledString 
    : public IHandle
    , public IRenderable {
    virtual void            SetPosition(uint32_t x, uint32_t y) = 0;
    virtual void            SetColor(const Vector3<float>& color) = 0;
    virtual std::wstring    GetText() const = 0;

    static LIB_EXPORT ICompiledString* CALLING_CONVENTION Create(IRenderContext* renderContext, IFontDecl* font, const std::wstring& text);
};
