#include "RenderContext.h"
#include "Sprite.h"
#include "FontRenderer.h"
#include "Utils.h"

int main() {
    try {
        IRenderContextBuilder* contextBuilder = IRenderContextBuilder::Create();
        contextBuilder->SetWidth(640);
        contextBuilder->SetHeight(480);
        contextBuilder->SetTitle("Sample");
        contextBuilder->SetApiType(RC_API_DX);
        IRenderContext* renderContext = contextBuilder->GetResult();
        IWindowContext* windowContext = renderContext->GetWindowContext();
        //std::wstring textureFile = Utils::Internal::GetMediaFolderPath() + std::wstring(L"Textures/Smile.png");
        //ITexture2D* texture = renderContext->CreateTexture2D(textureFile);
        // Create Sprite
        //ISprite* sprite = ISprite::Create(renderContext);
        //sprite->SetSize(100, 100);
        //sprite->SetTexture(texture);
        // Create Text
        IFontDecl* font = IFontDecl::Create("Arial", 12);
        IFontRenderer* fontRenderer = IFontRenderer::Create(renderContext);
        fontRenderer->SetFont(font);
        ICompiledString* text = fontRenderer->CompileString(L"Font rendering is guano!");
        int32_t textPosition = 0;
        // Render
        while(windowContext->ProcessMessage()) {
            renderContext->Clear(CLEAR_FLAG_COLOR|CLEAR_FLAG_DEPTH|CLEAR_FLAG_STENCIL, 255, 0, 0, 255, 1.0f, 0);
            //sprite->Render();
            //sprite->SetSize(sprite->GetWidth() + 1, sprite->GetHeight() + 1);
            //sprite->SetPosition(sprite->GetX() + 1, sprite->GetY() + 1);
            text->SetPosition(0, textPosition);
            text->Render();
            renderContext->Present();
            textPosition++;
        }
        //font->Release();
        //fontRenderer->Release();
        text->Release();
        //sprite->Release();
        renderContext->Release();
        contextBuilder->Release();
    }
    catch (std::exception& ex) {
        ex;
        return -1;
    }
    return 0;
}