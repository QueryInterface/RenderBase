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
        contextBuilder->SetApiType(RC_API_GLES2);
        IRenderContext* renderContext = contextBuilder->GetResult();
        IWindow* window = renderContext->GetWindow();
        // Render
        while(window->ProcessMessage()) {
        }
        renderContext->Release();
        contextBuilder->Release();
    }
    catch (std::exception& ex) {
        ex;
        return -1;
    }
    return 0;
}