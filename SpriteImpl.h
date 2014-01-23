#pragma once
#include "Sprite.h"
#include "RenderContextImpl.h"

#ifdef _WIN32
class SpriteDX9 : public ISprite {
public:
    SpriteDX9(RenderContextDX9* renderContext);
    virtual ~SpriteDX9();
    // IHandle
    virtual void Release();
    // IRenderable
    virtual void Render();
    // ISprite
    void            SetPosition(uint32_t x, uint32_t y);
    void            SetSize(uint32_t width, uint32_t height);
    void            SetColor(const Vector3<float>& color);
    void            SetOpacity(float a);
    void            SetTexture(ITexture2D* texture);
    void            SetTextureCoords(const Rect<float>& rect);

    uint32_t        GetX() const;
    uint32_t        GetY() const;
    uint32_t        GetWidth() const;
    uint32_t        GetHeight() const;
    Vector3<float>  GetColor() const;
    float           GetOpacity() const;
    ITexture2D*     GetTexture() const;
    Rect<float>     GetTextureCoords() const;
private:
    struct Vertex {
        float X, Y, Z, W;
        float U, V;
    };

    RenderContextDX9*   _renderContext;
    uint32_t            _width;
    uint32_t            _height;
    uint32_t            _x;
    uint32_t            _y;
    Vector4<float>      _color;
    Texture2DDX9*       _texture;
    Rect<float>         _textureCoords;

    CComPtr<IDirect3DVertexDeclaration9>    _vertexDeclaration;
    CComPtr<IDirect3DVertexBuffer9>         _vertexBuffer;
    CComPtr<IDirect3DVertexShader9>         _vertexShader;
    CComPtr<IDirect3DPixelShader9>          _pixelShader;
    CComPtr<ID3DXConstantTable>             _vsConstantTable;
    CComPtr<ID3DXConstantTable>             _psConstantTable;

    bool                                    _dirtyAdjustment;
    uint32_t                                _adjustedForWidth;
    uint32_t                                _adjustedForHeight;
    Vertex                                  _vertices[4];
    D3DXMATRIX                              _scaleMatrix;
    D3DXMATRIX                              _translateMatrix;
    D3DXMATRIX                              _resultMatrix;

    void            updateRelativeDimensions();
};
#endif //_WIN32