#include "Application.h"
#include "DebugLog.h"

#include <chrono>
#include <cmath>
#include <thread>

Application::Application(HINSTANCE instance, int showCommand)
    : window_(instance, showCommand, config_.windowTitle, config_.windowWidth, config_.windowHeight)
{
    renderer_.Initialize(window_.GetHandle(), window_.GetClientWidth(), window_.GetClientHeight());
    renderer_.SetVSyncEnabled(config_.vSyncEnabled);
    DebugLog::Format(
        "Frame pacing ready. VSync=%s TargetFPS=%u",
        renderer_.IsVSyncEnabled() ? "On" : "Off",
        config_.targetFrameRate);
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

    ApplyCursorSettings();
}

int Application::Run()
{
    timer_.Reset();
    DebugLog::Info("Main loop started.");

#if defined(_DEBUG)
    double nextStatusTime = config_.statusLogIntervalSeconds;
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

        if (input_.WasKeyPressed(config_.closeKey))
        {
            DebugLog::Info("Close key pressed. Closing.");
            window_.RequestClose();
        }

        if (input_.WasKeyPressed(config_.cursorLockToggleKey))
        {
            ToggleCursorLock();
        }

        if (input_.WasLeftMouseButtonPressed())
        {
            const auto mousePosition = input_.GetMousePosition();
            DebugLog::Format("Left click. X=%d Y=%d", mousePosition.x, mousePosition.y);
        }

        const float pulse = static_cast<float>((std::sin(timer_.GetTotalTime()) + 1.0) * 0.5);
        renderer_.BeginFrame(
            config_.clearColorRed,
            config_.clearColorGreen,
            config_.clearColorBlue + pulse * config_.clearColorPulseAmount,
            config_.clearColorAlpha);
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
            nextStatusTime += config_.statusLogIntervalSeconds;
        }
#endif

        LimitFrameRate(frameStartTime);
    }

    DebugLog::Info("Main loop stopped.");
    return 0;
}

void Application::LimitFrameRate(std::chrono::steady_clock::time_point frameStartTime) const
{
    if (config_.targetFrameRate == 0)
    {
        return;
    }

    const auto targetFrameTime = std::chrono::duration<double>(1.0 / config_.targetFrameRate);
    const auto elapsedTime = std::chrono::steady_clock::now() - frameStartTime;

    if (elapsedTime < targetFrameTime)
    {
        std::this_thread::sleep_for(targetFrameTime - elapsedTime);
    }
}

void Application::ApplyCursorSettings()
{
    window_.SetCursorVisible(config_.showCursor);

    if (config_.lockCursorOnStart)
    {
        window_.LockCursorToClient();
        DebugLog::Info("Cursor locked to window.");
    }
    else
    {
        window_.UnlockCursor();
        DebugLog::Info("Cursor unlocked.");
    }
}

void Application::ToggleCursorLock()
{
    if (window_.IsCursorLocked())
    {
        window_.UnlockCursor();
        DebugLog::Info("Cursor unlocked.");
    }
    else
    {
        window_.LockCursorToClient();
        DebugLog::Info("Cursor locked to window.");
    }
}
