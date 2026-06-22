#include "Application.h"

#include <cmath>

Application::Application(HINSTANCE instance, int showCommand)
    : window_(instance, showCommand, L"Apex Small Fry Duo", 1280, 720)
{
    renderer_.Initialize(window_.GetHandle(), window_.GetClientWidth(), window_.GetClientHeight());

    window_.SetResizeCallback([this](UINT width, UINT height)
    {
        renderer_.Resize(width, height);
    });

    window_.SetKeyCallback([this](UINT virtualKey, bool isDown)
    {
        input_.SetKeyState(virtualKey, isDown);
    });

    window_.SetFocusLostCallback([this]()
    {
        input_.Reset();
    });
}

int Application::Run()
{
    timer_.Reset();

    while (true)
    {
        input_.BeginFrame();

        if (!window_.ProcessMessages())
        {
            break;
        }

        timer_.Tick();

        if (input_.WasKeyPressed(VK_ESCAPE))
        {
            window_.RequestClose();
        }

        const float pulse = static_cast<float>((std::sin(timer_.GetTotalTime()) + 1.0) * 0.5);
        renderer_.Clear(0.08f, 0.12f, 0.15f + pulse * 0.12f, 1.0f);
        renderer_.Present();
    }

    return 0;
}
