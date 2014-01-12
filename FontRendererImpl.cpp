#include "FontRendererImpl.h"
#include "RenderContextImpl.h"
#include "ft2build.h"
#include "Sprite.h"
#include "ErrorHandler.h"

using namespace std::placeholders

static IFont* IFont::Create(const std::string& name, uint32_t size) {
    return new FontFT(name, size);
}

ICompiledString* CreateCompiledString(std::shared_ptr<IFontAtlas> fontAtlas, const Vector3<float>& color, const std::wstring& text) {
    RenderContextDX9* renderContextDX9 = dynamic_cast<RenderContextDX9*>(renderContext);
    if (renderContextDX9) {
        return new CompileStringDX9(fontAtlas, color, text);
    }    
}

std::shared_ptr<IFontAtlas> CreateFontAtlas(IRenderContext* renderContext) {
    RenderContextDX9* renderContextDX9 = dynamic_cast<RenderContextDX9*>(renderContext);
    if (renderContextDX9) {
        return dynamic_cast_pointer<IFontAtlas>(std::make_shared<FontAtlasDX9>(renderContextDX9));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FontFT /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontFT::FontFT(const std::string& name, uint32_t size) 
    : _name(name)
    , _size(size) {
    std::wstring fontPath = findFont(name);
    VE_ERROR_IF(fontPath.empty(), "Failed to find font %s", name);
    VE_ERROR_IF(FT_Init_FreeType(&_fontLibrary), "Failed to init font");
    VE_ERROR_IF(FT_New_Face(_fontLibrary, fontPath.c_str(), 0, &_face), "Failed to create font %s", name.c_str());
    VE_ERROR_IF(FT_Set_Pixel_Sizes(_face, 0, _size), "Failed to set font size %d", _size);
}

FontFT::~FontFT() {
    FT_Done_Face(_face);
    FT_DONE_FreeType(_fontLibrary);
}

void FontFT::Release() {
    delete this;
}

bool FontFT::GetGlyph(wchar_t c, Glyph& outGlyph) {
    // Try to find glyph
    auto glyphIter = _glyphs.find(c);
    if (glyphIter != _glyphs.end()) {
        outGlyph = glyphIter.second;
        return true;
    }
    // Glyph not found. Render it
    FT_Error error = FT_Load_Char(_face, c, FT_LOAD_RENDER); 
    VE_WARNING_IF(error, "Failed to load character");
    if (!error) {
        outGlyph.Width = _face->glyph->bitmap.width;
        outGlyph.Height = _face->glyph->bitmap.rows;
        outGlyph.OriginX = _face->glyph->bitmap_left;
        outGlyph.OriginY = _face->glyph->bitmap_top;
        outGlyph.StepX = _face->glyph->advance.x / 64;
        outGlyph.StepY = _face->glyph->advance.y / 64;
        _glyphs[c] = outGlyph;
        return true;
    }
    return false;
}

inline std::string FontFT::GetName() const {
    return _name;
}

uint32_t FontFT::GetSize() const {
    return _size;
}

#ifdef _WIN32
std::string FontFT::findFont(const std::string& fontName) {
    static const LPWSTR fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    HKEY hKey;
    std::wstring wsfontName(faceName.begin(), faceName.end());

    // Open Windows font registry key
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey)) {
        return "";
    }

    DWORD maxValueNameSize, maxValueDataSize;
    if (ERROR_SUCCESS != RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0)) {
        return "";
    }

    DWORD valueIndex = 0;
    std::vector<LPWSTR> valueName(maxValueNameSize);
    std::vector<LPBYTE> valueData(maxValueDataSize);
    DWORD valueNameSize, valueDataSize, valueType;
    std::wstring wsFontFile;

    LONG result = ERROR_SUCCESS;
    do {
        wsFontFile.clear();
        valueDataSize = maxValueDataSize;
        valueNameSize = maxValueNameSize;
        LONG result = RegEnumValue(hKey, valueIndex, valueName.data(), &valueNameSize, 0, &valueType, valueData.data(), &valueDataSize);

        valueIndex++;

        if (result != ERROR_SUCCESS || valueType != REG_SZ) {
            continue;
        }

        std::wstring wsValueName(valueName.data(), valueNameSize);

        if (wsFaceName.equal(wsValueName)) {
            wsFontFile.assign((LPWSTR)valueData.data(), valueDataSize);
            break;
        }
    } while (result != ERROR_NO_MORE_ITEMS);

    RegCloseKey(hKey);

    if (wsFontFile.empty()) {
        return "";
    }

    WCHAR winDir[MAX_PATH];
    GetWindowsDirectory(winDir, MAX_PATH);

    std::wstringstream ss;
    ss << winDir << "\\Fonts\\" << wsFontFile;
    wsFontFile = ss.str();

  return std::string(wsFontFile.begin(), wsFontFile.end());
}
#endif //_WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CompileStringDX9 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CompiledStringDX9::CompiledStringDX9(std::shared_ptr<IFontAtlas> fontAtlas, const Vector3<float>& color, const std::wstring& text) 
    : _fontAtlas(static_cast_pointer<FontAtlasDX9>(fontAtlas))
    , _color(color)
    , _text(text) {
    _fontAtlas->RegisterOnSizeChangeCallback(std::bind(&CompiledStringDX9::OnFontAtlasSizeChange, this, _1, _2));
}

CompiledStringDX9::~CompiledStringDX9() {
}

void CompiledStringDX9::Release()  {
    delete this;
}

void CompiledStringDX9::Render() {

}

void CompiledStringDX9::SetPosition(uint32_t x, uint32_t y) {

}

std::wstring CompiledStringDX9::GetText() const {
    return _text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FontAtlasDX9 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CompiledStringBuilder //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CompiledStringBuilder::CompiledStringBuilder(IRenderContext* renderContext) 
    : _color(1., 1., 1.)
    , _renderContext(renderContext) {
    _fontAtlas = CreateFontAtlas(renderContext);
}

CompiledStringBuilder::~CompiledStringBuilder() {
}

void CompiledStringBuilder::Release() {
    delete this;
}

inline svoid CompiledStringBuilder::SetFont(const IFont* font) {
    _fontAtlas->SetFont(font);
}

void CompiledStringBuilder::SetColor(const Vector3<float>& color) {
    _color = color;
}

ICompiledString* CompiledStringBuilder::CompileString(const std::wstring& text) {
    return CreateCompiledString(_fontAtlas, _color, text);
}