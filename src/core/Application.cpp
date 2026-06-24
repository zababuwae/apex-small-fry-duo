#include "Application.h"
#include "DebugLog.h"

#include <chrono>
#include <cmath>
#include <thread>

Application::Application(HINSTANCE instance, int showCommand)
    : window_(instance, showCommand, L"Apex Small Fry Duo", 1280, 720)
{
    renderer_.Initialize(window_.GetHandle(), window_.GetClientWidth(), window_.GetClientHeight());
    renderer_.SetVSyncEnabled(DefaultVSyncEnabled);
    DebugLog::Format(
        "Frame pacing ready. VSync=%s TargetFPS=%u",
        renderer_.IsVSyncEnabled() ? "On" : "Off",
        targetFrameRate_);
    DebugLog::Info("Core systems ready.");

    window_.SetResizeCallback([this](UINT width, UINT height)
    {
        renderer_.Resize(width, height);
    });

    window_.SetKeyCallback([this](UINT virtualKey, bool isDown)
    {
        input_.SetKeyState(virtualKey, isDown);
    });

    window_.SetMouseMoveCallback([this](int x, int y)
    {
        input_.SetMousePosition(x, y);
    });

    window_.SetLeftMouseButtonCallback([this](bool isDown, int x, int y)
    {
        input_.SetMousePosition(x, y);
        input_.SetLeftMouseButtonState(isDown);
    });

    window_.SetFocusLostCallback([this]()
    {
        input_.Reset();
        DebugLog::Info("Input reset.");
    });

    window_.SetCursorVisible(true);
    window_.LockCursorToClient();
    DebugLog::Info("Cursor locked to window.");
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
        const auto frameStartTime = std::chrono::steady_clock::now();

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

        if (input_.WasLeftMouseButtonPressed())
        {
            const auto mousePosition = input_.GetMousePosition();
            DebugLog::Format("Left click. X=%d Y=%d", mousePosition.x, mousePosition.y);
        }

        const float pulse = static_cast<float>((std::sin(timer_.GetTotalTime()) + 1.0) * 0.5);
        renderer_.BeginFrame(0.08f, 0.12f, 0.15f + pulse * 0.12f, 1.0f);
        // Rendering work can be added here.
        renderer_.EndFrame();

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

        LimitFrameRate(frameStartTime);
    }

    DebugLog::Info("Main loop stopped.");
    return 0;
}

void Application::LimitFrameRate(std::chrono::steady_clock::time_point frameStartTime) const
{
    if (targetFrameRate_ == 0)
    {
        return;
    }

    const auto targetFrameTime = std::chrono::duration<double>(1.0 / targetFrameRate_);
    const auto elapsedTime = std::chrono::steady_clock::now() - frameStartTime;

    if (elapsedTime < targetFrameTime)
    {
        std::this_thread::sleep_for(targetFrameTime - elapsedTime);
    }
}
