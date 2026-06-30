#pragma once

#include "EngineConfig.h"
#include "Timer.h"
#include "input/Input.h"
#include "platform/Window.h"
#include "renderer/Renderer.h"

#include <Windows.h>

#include <chrono>

class Application final
{
public:
    Application(HINSTANCE instance, int showCommand);

    int Run();

private:
    void LimitFrameRate(std::chrono::steady_clock::time_point frameStartTime) const;
    void ApplyCursorSettings();
    void ToggleCursorLock();

    EngineConfig config_;
    Window window_;
    Renderer renderer_;
    Timer timer_;
    Input input_;
};
