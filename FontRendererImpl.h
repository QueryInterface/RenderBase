#pragma once
#include "FontRenderer.h"

class FontFT : public IFont {
public:
    FontFT(const std::string& name, uint32_t size);
    virtual ~FontFT();
    // IHandle
    virtual void Release();
    // FontFT
    virtual bool        GetGlyph(wchar_t c, Glyph& outGlyph);
    virtual std::string GetName() const;
    virtual uint32_t    GetSize() const;
private:
    std::string                     _name;
    uint32_t                        _size;
    FT_Library                      _fontLibrary;
    FT_Face                         _face;
    std::map<wchar_t, IFont::Glyph> _glyphs;

    std::string findFont(const std::string& fontName);
};

class CompiledStringDX9 : public ICompiledString {
public:
    CompiledStringDX9(IFont* font, const Vector3<float>& color, const std::wstring& text);
    virtual ~CompiledStringDX9();
    // IHandle
    virtual void            Release();
    // IRenderable
    virtual void            Render();
    // ICompiledString
    virtual void            SetPosition(uint32_t x, uint32_t y);
    virtual std::wstring    GetText() const;
private:
    std::wstring    _text;
    Vector3<float>  _color;
};

class CompiledStringBuilder : public ICompiledStringBuilder {
public:
    CompiledStringBuilder(IRenderContext* renderContext);
    virtual ~CompiledStringBuilder();
    // IHandle
    virtual void                Release();
    // ICompiledStringBuilder
    virtual void                SetFont(const IFont* font) = 0;
    virtual void                SetColor(const Vector3<float>& color) = 0;
    virtual ICompiledString*    CompileString(const std::wstring& text) = 0;
private:
    const IFont*    _font;
    Vector3<float>  _color;
    IRenderContext* _renderContext;
}