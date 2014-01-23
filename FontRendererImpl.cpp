#include "FontRendererImpl.h"
#include "ErrorHandler.h"
#include <sstream>
#include "RenderContextImpl.h"

IFontDecl* IFontDecl::Create(const std::string& name, uint32_t size) {
    return new FontFT(name, size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FontFT /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontFT::FontFT(const std::string& name, uint32_t size) 
    : _name(name)
    , _size(size)
    , _kerningEnabled(false) 
    , _lineSpace(0) {
    std::wstring fontPath = findFont(name);
    std::string fontPathA(fontPath.begin(), fontPath.end());
    VE_ERROR_IF(fontPath.empty(), L"Failed to find font %s", name);
    VE_ERROR_IF(FT_Init_FreeType(&_fontLibrary), L"Failed to init font");
    VE_ERROR_IF(FT_New_Face(_fontLibrary, fontPathA.c_str(), 0, &_face), L"Failed to create font %S", name.c_str());
    // TODO: FT_Set_Char_Size here isntead for dpi support
    VE_ERROR_IF(FT_Set_Pixel_Sizes(_face, 0, _size), L"Failed to set font size %d", _size);
    _kerningEnabled = (bool)FT_HAS_KERNING(_face);
    _lineSpace = _face->height;
}

FontFT::~FontFT() {
    FT_Done_Face(_face);
    FT_Done_FreeType(_fontLibrary);
}

void FontFT::Release() {
    delete this;
}

bool FontFT::GetGlyph(wchar_t c, Glyph& outGlyph) {
    // Try to find glyph
    auto glyphIter = _glyphs.find(c);
    if (glyphIter != _glyphs.end()) {
        outGlyph = glyphIter->second;
        return true;
    }
    // Glyph not found. Render it
    FT_Error error = FT_Load_Char(_face, c, FT_LOAD_RENDER); 
    VE_WARNING_IF(error, L"Failed to load character");
    if (!error) {
        outGlyph.Width = _face->glyph->bitmap.width;
        outGlyph.Height = _face->glyph->bitmap.rows;
        outGlyph.OriginX = _face->glyph->bitmap_left;
        outGlyph.OriginY = _face->glyph->bitmap_top;
        outGlyph.StepX = _face->glyph->advance.x >> 6;
        outGlyph.StepY = _face->glyph->advance.y >> 6;
        outGlyph.Font = this;
        outGlyph.Symbol = c;
        uint8_t* src = _face->glyph->bitmap.buffer;
        outGlyph.Data.reserve(outGlyph.Width * outGlyph.Height);
        for (uint32_t h = 0; h < outGlyph.Height; ++h) {
            for (uint32_t w = 0; w < outGlyph.Width; ++w) {
                outGlyph.Data.push_back(src[w]);
            }
            src += _face->glyph->bitmap.pitch;
        }
        _glyphs[c] = outGlyph;
        return true;
    }
    return false;
}

inline std::string FontFT::GetName() const {
    return _name;
}

inline uint32_t FontFT::GetLineSpace() const {
    return _lineSpace;
}

uint32_t FontFT::GetSize() const {
    return _size;
}

uint32_t FontFT::GetKerning(wchar_t c0, wchar_t c1) const {
    if (!_kerningEnabled) {
        return 0;
    }
    FT_UInt c0Index = FT_Get_Char_Index(_face, c0);
    FT_UInt c1Index = FT_Get_Char_Index(_face, c1);
    FT_Vector delta;
    FT_Get_Kerning(_face, c0Index, c1Index, FT_KERNING_DEFAULT, &delta);
    return delta.x >> 6;
}

#ifdef _WIN32
std::wstring FontFT::findFont(const std::string& fontName) {
    std::wstring wsFontFile;
    static const LPCSTR fontRegistryPath = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    HKEY hKey;
    std::wstring wsfontName(fontName.begin(), fontName.end());

    // Open Windows font registry key
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey)) {
        return wsFontFile;
    }

    DWORD maxValueNameSize, maxValueDataSize;
    if (ERROR_SUCCESS != RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0)) {
        return wsFontFile;
    }

    DWORD valueIndex = 0;
    std::vector<wchar_t> valueName(maxValueNameSize);
    std::vector<uint8_t> valueData(maxValueDataSize);
    DWORD valueNameSize, valueDataSize, valueType;

    LONG result = ERROR_SUCCESS;
    do {
        valueDataSize = maxValueDataSize;
        valueNameSize = maxValueNameSize;
        LONG result = RegEnumValueW(hKey, valueIndex, valueName.data(), &valueNameSize, 0, &valueType, valueData.data(), &valueDataSize);

        valueIndex++;

        if (result != ERROR_SUCCESS || valueType != REG_SZ) {
            continue;
        }

        std::wstring wsValueName = valueName.data();

        if (wsfontName.compare(wsValueName) != std::string::npos) {
            wsFontFile.assign((LPWSTR)valueData.data(), valueDataSize);
            break;
        }
    } while (result != ERROR_NO_MORE_ITEMS);

    RegCloseKey(hKey);

    if (wsFontFile.empty()) {
        return wsFontFile;
    }

    WCHAR winDir[MAX_PATH];
    GetWindowsDirectoryW(winDir, MAX_PATH);

    std::wstringstream ss;
    ss << winDir << L"\\Fonts\\" << wsFontFile;
    wsFontFile = ss.str();

    return wsFontFile;
}
#endif //_WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CompileStringDX9 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const std::string g_vertexShaderSourceDX9 = 
    "struct VS_INPUT {                                      \n\
        float2 Position : POSITION;                         \n\
        float2 TexCoord : TEXCOORD0;                        \n\
    };                                                      \n\
                                                            \n\
    struct VS_OUTPUT {                                      \n\
        float4 Position : POSITION;                         \n\
        float2 TexCoord : TEXCOORD0;                        \n\
    };                                                      \n\
                                                            \n\
    float2  g_shift;                                        \n\
    float   g_w;      //Screen width                        \n\
    float   g_h;      //Screen height                       \n\
                                                            \n\
    VS_OUTPUT vs_main (VS_INPUT input) {                    \n\
        VS_OUTPUT output;                                   \n\
        float x = input.Position / g_w - 1.0f + g_shift.x;  \n\
        float y = input.Position / g_h- 1.0f + g_shift.y;   \n\
        output.Position = float4(x, y, 0.5, 1.0);           \n\
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
CompiledStringDX9::CompiledStringDX9(RenderContextDX9* renderContext, IFontDecl* font, FontAtlasPtr fontAtlas, const std::wstring& text) 
    : _renderContext(renderContext)
    , _font(font)
    , _fontAtlas(fontAtlas)
    , _text(text) {
    HRESULT hr = S_OK;
    float penX = 0.;
    float penY = 0.;
    uint32_t kerning = 0;
    wchar_t prev = 0;
    for (wchar_t c : text) {
        IFontDecl::Glyph glyph;
        _font->GetGlyph(c, glyph);
        uint32_t size = _font->GetSize();
        uint32_t kerning = prev ? _font->GetKerning(prev, c) : 0;
        Rect<float> texCoords = _fontAtlas->GetTextureCoords(glyph);
        Vertex v;
        // 1st triangle
        v.X = penX + glyph.OriginX;                   v.U = texCoords.Left;
        v.Y = penY - glyph.OriginY;                   v.V = texCoords.Top;
        _vertices.push_back(v);
        v.X = penX + glyph.OriginX + glyph.Width;     v.U = texCoords.Right;
        v.Y = penY - glyph.OriginY;                   v.V = texCoords.Top;
        _vertices.push_back(v);
        v.X = penX + glyph.OriginX;                   v.U = texCoords.Left;
        v.Y = penY - glyph.OriginY + glyph.Height;    v.V = texCoords.Bottom;
        _vertices.push_back(v);
        // 2nd triangle
        v.X = penX + glyph.OriginX;                   v.U = texCoords.Left;
        v.Y = penY - glyph.OriginY + glyph.Height;    v.V = texCoords.Bottom;
        _vertices.push_back(v);
        v.X = penX + glyph.OriginX + glyph.Width;     v.U = texCoords.Right;
        v.Y = penY - glyph.OriginY;                   v.V = texCoords.Top;
        _vertices.push_back(v);
        v.X = penX + glyph.OriginX + glyph.Width;     v.U = texCoords.Right;
        v.Y = penY - glyph.OriginX + glyph.Height;    v.V = texCoords.Bottom;
        _vertices.push_back(v);

        penX += glyph.StepX;
    }
    // Create vertex declaration
    D3DVERTEXELEMENT9 elements[] = {
        {0, 0,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
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
    memcpy(pData, _vertices.data(), _vertices.size() * sizeof(Vertex));
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
}

CompiledStringDX9::~CompiledStringDX9() {
}

void CompiledStringDX9::Release()  {
    delete this;
}

void CompiledStringDX9::Render() {
    CComPtr<IDirect3DDevice9>& device = _renderContext->GetDevice();
    device->BeginScene();
    _vsConstantTable->SetFloatArray(device, "g_shift", _position.data(), 2);
    _vsConstantTable->SetFloat(device, "g_w", (float)_renderContext->GetWindowContext()->GetWidth());
    _vsConstantTable->SetFloat(device, "g_h", (float)_renderContext->GetWindowContext()->GetHeight());
    _psConstantTable->SetFloatArray(device, "Color", (float*)&_color, 4);
    device->SetVertexShader(_vertexShader);
    device->SetPixelShader(_pixelShader);
    device->SetTexture(0, static_cast<Texture2DDX9*>(_fontAtlas->GetTexture())->GetPointer());
    device->SetVertexDeclaration(_vertexDeclaration);
    device->SetStreamSource(0, _vertexBuffer, 0, sizeof(Vertex));
    device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, _vertices.size() / 3);
    device->EndScene();    
}

void CompiledStringDX9::SetPosition(uint32_t x, uint32_t y) {
    _position.x = (float)x;
    _position.y = (float)y;
}

void CompiledStringDX9::SetColor(const Vector3<float>& color) {
    _color = color;
}

std::wstring CompiledStringDX9::GetText() const {
    return _text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FontAtlasDX9 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontAtlas::FontAtlas(IRenderContext* renderContext) 
    : _renderContext(renderContext) 
    , _atlas(nullptr) {
    _atlas = _renderContext->CreateTexture2D(1, 512, 512, TEX_FORMAT_A8, TEX_FLAG_WRITE);
    VE_ERROR_IF(_atlas == nullptr, L"Failed to create texture atlas");
}

FontAtlas::~FontAtlas() {
    _atlas->Release();
}

inline ITexture2D* FontAtlas::GetTexture() const {
    return _atlas;
}

Rect<float> FontAtlas::GetTextureCoords(const IFontDecl::Glyph& glyph) {
    Rect<float> r;
    r.Bottom = r.Left = r.Right = r.Top = 0.;
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FontRenrerer //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontRenderer::FontRenderer(IRenderContext* renderContext) 
    : _color(1., 1., 1.)
    , _renderContext(renderContext) {
    RenderContextDX9* renderContextDX9 = dynamic_cast<RenderContextDX9*>(renderContext);
    if (renderContextDX9) {
        _fontAtlas = std::make_shared<FontAtlas>(renderContextDX9);
    }
}

FontRenderer::~FontRenderer() {
}

void FontRenderer::Release() {
    delete this;
}

inline void FontRenderer::SetFont(IFontDecl* font) {
    _font = font;
}

void FontRenderer::SetColor(const Vector3<float>& color) {
    _color = color;
}

ICompiledString* FontRenderer::CompileString(const std::wstring& text) {
    ICompiledString* string;
    RenderContextDX9* renderContextDX9 = dynamic_cast<RenderContextDX9*>(_renderContext);
    if (renderContextDX9) {
        string = new CompiledStringDX9(renderContextDX9, _font, _fontAtlas, text);
    } 
    string->SetColor(_color);
    return string;
}

IFontRenderer* IFontRenderer::Create(IRenderContext* renderContext) {
    return new FontRenderer(renderContext);
}