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
        bool b = true;
        while(b) {
            WINDOW_MSG msg = window->ProcessMessage();
            switch (msg) {
            case WINDOW_MSG::FOREGROUND:
                {
                    // Render here
                }
                break;
            case WINDOW_MSG::QUIT:
                b = false;
            }
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
