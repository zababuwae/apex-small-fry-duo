#include "Application.h"

#include <stdexcept>

Application::Application(HINSTANCE instance, int showCommand)
    : window_(instance, showCommand, L"Apex Small Fry Duo", 1280, 720)
{
    renderer_.Initialize(window_.GetHandle(), window_.GetClientWidth(), window_.GetClientHeight());

    window_.SetResizeCallback([this](UINT width, UINT height)
    {
        renderer_.Resize(width, height);
    });
}

int Application::Run()
{
    while (window_.ProcessMessages())
    {
        renderer_.Clear(0.08f, 0.12f, 0.18f, 1.0f);
        renderer_.Present();
    }

    return 0;
}
