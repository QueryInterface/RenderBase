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
    CompiledStringDX9(std::shared_ptr<IFontAtlas> fontAtlas, const Vector3<float>& color, const std::wstring& text);
    virtual ~CompiledStringDX9();
    // IHandle
    virtual void            Release();
    // IRenderable
    virtual void            Render();
    // ICompiledString
    virtual void            SetPosition(uint32_t x, uint32_t y);
    virtual std::wstring    GetText() const;
    // CompiledStringDX9
    virtual void            OnFontAtlasSizeChange(float xRatio, float yRatio); 
private:
    std::wstring                    _text;
    Vector3<float>                  _color;
    std::shared_ptr<FontAtlasDX9>   _fontAtlas;
};

ICompiledString* CreateCompiledString(std::shared_ptr<IFontAtlas> fontAtlas, const Vector3<float>& color, const std::wstring& text);

class FontAtlasDX9 : public IFontAtlas {
public:
    FontAtlasDX9(RenderContextDX9* renderContext);
    virtual ~FontAtlasDX9();
    // IFontAtlas
    virtual void SetFont(IFont* font);
    virtual void RegisterOnSizeChangeCallback(const OnSizeChangeCallback_TYPE& f);
    // FontAtlasDX9
    CComPtr<Direct3DTexture9> GetTexture() const;
private:
    IFont*                                  _font;
    uint32_t                                _atlasWidth;
    uint32_t                                _atlasHeight;
    CComPtr<Direct3DTexture9>               _atlas;
    std::list<OnSizeChangeCallback_TYPE>    _onSizeChangeCallbacks;
    RenderContextDX9*                       _renderContext;
};

std::shared_ptr<IFontAtlas> CreateFontAtlas(IRenderContext* renderContext);

class CompiledStringBuilder : public ICompiledStringBuilder {
public:
    CompiledStringBuilder(IRenderContext* renderContext);
    virtual ~CompiledStringBuilder();
    // IHandle
    virtual void                Release();
    // ICompiledStringBuilder
    virtual void                SetFont(const IFont* font);
    virtual void                SetColor(const Vector3<float>& color);
    virtual ICompiledString*    CompileString(const std::wstring& text);
private:
    std::shared_ptr<IFontAtlas> _fontAtlas;
    Vector3<float>              _color;
    IRenderContext*             _renderContext;
}