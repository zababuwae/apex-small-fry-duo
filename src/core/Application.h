#pragma once

#include "../platform/Window.h"
#include "../renderer/Renderer.h"

#include <Windows.h>

class Application final
{
public:
    Application(HINSTANCE instance, int showCommand);

    int Run();

private:
    Window window_;
    Renderer renderer_;
};
