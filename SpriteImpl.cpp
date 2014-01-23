#include "SpriteImpl.h"
#include "RenderContextImpl.h"
#include "ErrorHandler.h"

#ifdef _WIN32
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SpriteDX9 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const std::string g_vertexShaderSourceDX9 = 
    "struct VS_INPUT {                                      \n\
        float4 Position : POSITION;                         \n\
        float2 TexCoord : TEXCOORD0;                        \n\
    };                                                      \n\
                                                            \n\
    struct VS_OUTPUT {                                      \n\
        float4 Position : POSITION;                         \n\
        float2 TexCoord : TEXCOORD0;                        \n\
    };                                                      \n\
                                                            \n\
    float4x4 g_matrix;                                      \n\
                                                            \n\
    VS_OUTPUT vs_main (VS_INPUT input) {                    \n\
        VS_OUTPUT output;                                   \n\
        output.Position = mul(input.Position, g_matrix);    \n\
        output.TexCoord = input.TexCoord;                   \n\
        return output;                                      \n\
    }                                                       \n";

static const std::string g_pixelShaderSourceDX9 = 
    "struct PS_INPUT {                                      \n\
        float4 Position : POSITION;                         \n\
        float2 TexCoord : TEXCOORD0;                        \n\
    };                                                      \n\
                                                            \n\
    struct PS_OUTPUT {                                      \n\
        float4 Color : COLOR0;                              \n\
    };                                                      \n\
                                                            \n\
    sampler2D Tex0;                                         \n\
    float4    Color;                                        \n\
                                                            \n\
    PS_OUTPUT ps_main(PS_INPUT input ) {                    \n\
        PS_OUTPUT output;                                   \n\
        float4 texColor = tex2D(Tex0, input.TexCoord);      \n\
        output.Color = Color * texColor;                    \n\
        return output;                                      \n\
    }                                                       \n";

SpriteDX9::SpriteDX9(RenderContextDX9* renderContext)
    : _renderContext(renderContext)
    , _width(10)
    , _height(10)
    , _x(0)
    , _y(0)
    , _color(1.0f, 1.0f, 1.0f, 1.0f)
    , _texture(nullptr)
    , _adjustedForWidth(0) 
    , _adjustedForHeight(0)
    , _dirtyAdjustment(true) {
    HRESULT hr = S_OK;
    _textureCoords.Left = 0.0f;
    _textureCoords.Top = 0.0f;
    _textureCoords.Bottom = 1.0f;
    _textureCoords.Right = 1.0f;
    // Create vertex data
    memset(&_vertices, 0, sizeof(_vertices)); 
    _vertices[0].Z = _vertices[1].Z = _vertices[2].Z = _vertices[3].Z = 0.5;
    _vertices[0].W = _vertices[1].W = _vertices[2].W = _vertices[3].W= 1.0;
    _vertices[0].U = _textureCoords.Left;
    _vertices[0].V = _textureCoords.Top;
    _vertices[1].U = _textureCoords.Right;
    _vertices[1].V = _textureCoords.Top;
    _vertices[2].U = _textureCoords.Left;
    _vertices[2].V = _textureCoords.Bottom;
    _vertices[3].U = _textureCoords.Right;
    _vertices[3].V = _textureCoords.Bottom;

    _vertices[0].X = 0.0f;
    _vertices[0].Y = 0.0f;
    _vertices[1].X = 1.0f;
    _vertices[1].Y = 0.0f;
    _vertices[2].X = 0.0f;
    _vertices[2].Y = -1.0f;
    _vertices[3].X = 1.0f;
    _vertices[3].Y = -1.0f;
    // Create vertex declaration
    D3DVERTEXELEMENT9 elements[] = {
        {0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
        D3DDECL_END()
    };
    CComPtr<IDirect3DDevice9> device = _renderContext->GetDevice();
    hr = device->CreateVertexDeclaration(elements, &_vertexDeclaration);                                   
    VE_ERROR_IF(FAILED(hr), L"Failed to create vertex declaration");
    // Create vertex buffer
    hr = device->CreateVertexBuffer(sizeof(_vertices), 0, 0, D3DPOOL_DEFAULT, &_vertexBuffer, NULL);      
    VE_ERROR_IF(FAILED(hr), L"Failed to create vertex buffer");
    void* pData = nullptr;
    hr = _vertexBuffer->Lock(0, sizeof(_vertices), &pData, 0);                                            
    VE_ERROR_IF(FAILED(hr), L"Failed to lock vertex buffer %p", _vertexBuffer); 
    memcpy(pData, _vertices, sizeof(_vertices));
    hr = _vertexBuffer->Unlock();                                                                          
    VE_ERROR_IF(FAILED(hr), L"Failed to unlock vertex buffer");
    // Create shaders
    CComPtr<ID3DXBuffer> shaderBinary;
    shaderBinary = _renderContext->CompileShader(g_vertexShaderSourceDX9, "vs_main", "vs_2_0", &_vsConstantTable);
    hr = device->CreateVertexShader((DWORD*)shaderBinary->GetBufferPointer(), &_vertexShader);             
    VE_ERROR_IF(FAILED(hr), L"Failed to create vertex shader");
    shaderBinary = _renderContext->CompileShader(g_pixelShaderSourceDX9, "ps_main", "ps_2_0", &_psConstantTable);
    hr = device->CreatePixelShader((DWORD*)shaderBinary->GetBufferPointer(), &_pixelShader);               
    VE_ERROR_IF(FAILED(hr), L"Failed to create pixel shader");
    // Set transformation matrix to identity
    D3DXMatrixIdentity(&_scaleMatrix);
    D3DXMatrixIdentity(&_translateMatrix);
    D3DXMatrixIdentity(&_resultMatrix);
}

SpriteDX9::~SpriteDX9() {
}

void SpriteDX9::Release() {
    delete this;
}

void SpriteDX9::Render() {
    updateRelativeDimensions();
    CComPtr<IDirect3DDevice9>& device = _renderContext->GetDevice();
    IDirect3DTexture9* texture = _texture ? _texture->GetPointer() : nullptr;
    device->BeginScene();
    _vsConstantTable->SetMatrix(device, "g_matrix", &_resultMatrix);
    _psConstantTable->SetFloatArray(device, "Color", (float*)&_color, 4);
    device->SetVertexShader(_vertexShader);
    device->SetPixelShader(_pixelShader);
    device->SetTexture(0, texture);
    device->SetVertexDeclaration(_vertexDeclaration);
    device->SetStreamSource(0, _vertexBuffer, 0, sizeof(Vertex));
    device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    device->EndScene();
}

inline void SpriteDX9::SetPosition(uint32_t x, uint32_t y) {
    _x = x;
    _y = y;
    // Invalidate adjustment
    _dirtyAdjustment = true;
}

inline void SpriteDX9::SetSize(uint32_t width, uint32_t height) {
    _width = width;
    _height = height;
    // Invalidate adjustment
    _dirtyAdjustment = true;
}

inline void SpriteDX9::SetColor(const Vector3<float>& color) {
    _color.x = color.x;
    _color.y = color.y;
    _color.z = color.z;
}

inline void SpriteDX9::SetOpacity(float a) {
    _color.w = a;
}

inline void SpriteDX9::SetTexture(ITexture2D* texture) {
    _texture = dynamic_cast<Texture2DDX9*>(texture);
}

inline void SpriteDX9::SetTextureCoords(const Rect<float>& rect) {
    _textureCoords = rect;
}

inline uint32_t SpriteDX9::GetX() const {
    return _x;
}

inline uint32_t SpriteDX9::GetY() const {
    return _y;
}

inline uint32_t SpriteDX9::GetWidth() const {
    return _width;
}

inline uint32_t SpriteDX9::GetHeight() const {
    return _height;
}

inline Vector3<float> SpriteDX9::GetColor() const {
    return Vector3<float>(_color.x, _color.y, _color.z);
}

inline float SpriteDX9::GetOpacity() const {
    return _color.w;
}

inline ITexture2D* SpriteDX9::GetTexture() const {
    return _texture;
}

inline Rect<float> SpriteDX9::GetTextureCoords() const {
    return _textureCoords;
}

void SpriteDX9::updateRelativeDimensions() {
    HRESULT hr = S_OK;
    CComPtr<IDirect3DDevice9>& device = _renderContext->GetDevice();
    CComPtr<IDirect3DSurface9> rt;
    hr = device->GetRenderTarget(0, &rt);                                   
    VE_ERROR_IF(FAILED(hr), L"Failed to get 0 render target");
    D3DSURFACE_DESC rtDesc;
    rt->GetDesc(&rtDesc);
    uint32_t rtWidth = rtDesc.Width;
    uint32_t rtHeight = rtDesc.Height;
    if (rtWidth == _adjustedForWidth && rtHeight == _adjustedForHeight && !_dirtyAdjustment) {
        // Already adjusted
        return;
    }
    // Adjust size
    float w = 2.0f * (float)_width / rtWidth;
    float h = 2.0f * (float)_height / rtHeight;
    D3DXMatrixScaling(&_scaleMatrix, w, h, 1.0f);
    D3DXMatrixMultiply(&_resultMatrix, &_scaleMatrix, &_translateMatrix);
    hr = _vsConstantTable->SetMatrix(device, "g_matrix", &_resultMatrix);
    VE_ERROR_IF(FAILED(hr), L"Failed to set g_matrix variable in VS shader");
    // Adjust position
    float x_shift = -1.0f + 2.0f * (float)_x / rtWidth;
    float y_shift = 1.0f - 2.0f * (float)_y / rtHeight;
    D3DXMatrixTranslation(&_translateMatrix, x_shift, y_shift, 0.0f);
    D3DXMatrixMultiply(&_resultMatrix, &_scaleMatrix, &_translateMatrix);
    hr = _vsConstantTable->SetMatrix(device, "g_matrix", &_resultMatrix);
    VE_ERROR_IF(FAILED(hr), L"Failed to set g_matrix variable in VS shader");
    // Store width and height for which adjustment was perform and mark adjatment being valid
    _adjustedForWidth = rtWidth;
    _adjustedForHeight = rtHeight;
    _dirtyAdjustment = false;
}

#endif //_WIN32

ISprite* ISprite::Create(IRenderContext* renderContext) {
    try {
#ifdef _WIN32
        RenderContextDX9* renderContextDX9 = dynamic_cast<RenderContextDX9*>(renderContext);
        if (renderContextDX9) return new SpriteDX9(renderContextDX9);
#endif //_WIN32
        VE_ERROR(L"Failed to create sprite. Incompartible render context type");
    }
    catch (...) {
    }
    return nullptr;
}