#include "RenderContext.h"
#include "Utils.h"

int main() {
    try {
        IRenderContextBuilder* contextBuilder = IRenderContextBuilder::Create();
        contextBuilder->SetWidth(640);
        contextBuilder->SetHeight(480);
        contextBuilder->SetTitle("Sample");
        contextBuilder->SetApiType(RC_API_TYPE::GLES2);
        IRenderContext* renderContext = contextBuilder->GetResult();
        IWindow* window = renderContext->GetWindow();
        // Render
        WINDOW_MSG msg = WINDOW_MSG::FOREGROUND;
        while((msg = window->ProcessMessage()) != WINDOW_MSG::QUIT) {
            if (msg == WINDOW_MSG::FOREGROUND)
            {
            }
            renderContext->Present();
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
