#pragma once
#include "FontRenderer.h"
#include "RenderContextImpl.h"
#include "ft2build.h"
#include <map>
#include <memory>
#include FT_FREETYPE_H

class FontAtlas;
typedef shared_ptr<FontAtlas> FontAtlasPtr;

class FontFT : public IFontDecl {
public:
    FontFT(const std::string& name, uint32_t size);
    virtual ~FontFT();
    // IHandle
    virtual void Release();
    // FontFT
    virtual bool        GetGlyph(wchar_t c, IFontDecl::Glyph& outGlyph);
    virtual std::string GetName() const;
    virtual uint32_t    GetLineSpace() const;
    virtual uint32_t    GetSize() const;
    virtual uint32_t    GetKerning(wchar_t c0, wchar_t c1) const;
private:
    std::string                         _name;
    uint32_t                            _lineSpace;
    bool                                _kerningEnabled;
    uint32_t                            _size;
    FT_Library                          _fontLibrary;
    FT_Face                             _face;
    std::map<wchar_t, IFontDecl::Glyph> _glyphs;

    std::wstring findFont(const std::string& fontName);
};

class CompiledStringDX9 : public ICompiledString {
public:
    CompiledStringDX9(RenderContextDX9* renderContext, IFontDecl* font, FontAtlasPtr fontAtlas, const std::wstring& text);
    virtual ~CompiledStringDX9();
    // IHandle
    virtual void            Release();
    // IRenderable
    virtual void            Render();
    // ICompiledString
    virtual void            SetPosition(uint32_t x, uint32_t y);
    virtual void            SetColor(const Vector3<float>& color);
    virtual std::wstring    GetText() const;
private:
    struct Vertex {
        float X, Y;
        float U, V;
    };
    std::vector<Vertex>     _vertices;

    std::wstring            _text;
    Vector2<float>       _position;
    Vector3<float>          _color;
    IFontDecl*              _font;
    FontAtlasPtr            _fontAtlas;
    RenderContextDX9*       _renderContext;

    CComPtr<IDirect3DVertexDeclaration9>    _vertexDeclaration;
    CComPtr<IDirect3DVertexBuffer9>         _vertexBuffer;
    CComPtr<IDirect3DVertexShader9>         _vertexShader;
    CComPtr<IDirect3DPixelShader9>          _pixelShader;
    CComPtr<ID3DXConstantTable>             _vsConstantTable;
    CComPtr<ID3DXConstantTable>             _psConstantTable;
};

class FontAtlas {
public:
    FontAtlas(IRenderContext* renderContext);
    virtual ~FontAtlas();
    // FontAtlasDX9
    ITexture2D* GetTexture() const;
    Rect<float> GetTextureCoords(const IFontDecl::Glyph& glyph);
private:
    ITexture2D*                 _atlas;
    IRenderContext*             _renderContext;
};

class FontRenderer : public IFontRenderer {
public:
    FontRenderer(IRenderContext* renderContext);
    virtual ~FontRenderer();
    // IHandle
    virtual void                Release();
    // ICompiledStringBuilder
    virtual void                SetFont(IFontDecl* font);
    virtual void                SetColor(const Vector3<float>& color);
    virtual ICompiledString*    CompileString(const std::wstring& text);
private:
    IFontDecl*                  _font;
    FontAtlasPtr                _fontAtlas;
    Vector3<float>              _color;
    IRenderContext*             _renderContext;
};