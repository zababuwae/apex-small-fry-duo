#pragma once

#include "Timer.h"
#include "../input/Input.h"
#include "../platform/Window.h"
#include "../renderer/Renderer.h"

#include <Windows.h>

#include <chrono>

class Application final
{
public:
    Application(HINSTANCE instance, int showCommand);

    int Run();

private:
    void LimitFrameRate(std::chrono::steady_clock::time_point frameStartTime) const;

    static constexpr bool DefaultVSyncEnabled = true;
    static constexpr unsigned int DefaultTargetFrameRate = 60;

    Window window_;
    Renderer renderer_;
    Timer timer_;
    Input input_;
    unsigned int targetFrameRate_ = DefaultTargetFrameRate;
};
