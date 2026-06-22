#include "Application.h"
#include "DebugLog.h"

#include <cmath>

Application::Application(HINSTANCE instance, int showCommand)
    : window_(instance, showCommand, L"Apex Small Fry Duo", 1280, 720)
{
    renderer_.Initialize(window_.GetHandle(), window_.GetClientWidth(), window_.GetClientHeight());
    DebugLog::Info("Core systems ready.");

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
        DebugLog::Info("Input reset.");
    });
}

int Application::Run()
{
    timer_.Reset();
    DebugLog::Info("Main loop started.");

#if defined(_DEBUG)
    double nextStatusTime = 1.0;
    unsigned int frameCount = 0;
#endif

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
            DebugLog::Info("Escape pressed. Closing.");
            window_.RequestClose();
        }

        const float pulse = static_cast<float>((std::sin(timer_.GetTotalTime()) + 1.0) * 0.5);
        renderer_.Clear(0.08f, 0.12f, 0.15f + pulse * 0.12f, 1.0f);
        renderer_.Present();

#if defined(_DEBUG)
        ++frameCount;
        if (timer_.GetTotalTime() >= nextStatusTime)
        {
            DebugLog::Format(
                "Running. FPS=%u Delta=%.3f ms",
                frameCount,
                timer_.GetDeltaTime() * 1000.0);

            frameCount = 0;
            nextStatusTime += 1.0;
        }
#endif
    }

    DebugLog::Info("Main loop stopped.");
    return 0;
}
