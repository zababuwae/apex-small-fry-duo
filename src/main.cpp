#include "core/Application.h"

#include <Windows.h>

#include <cstdlib>
#include <exception>

int WINAPI wWinMain(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    PWSTR commandLine,
    int showCommand)
{
    (void)previousInstance;
    (void)commandLine;

    try
    {
        Application application(instance, showCommand);
        return application.Run();
    }
    catch (const std::exception& exception)
    {
        MessageBoxA(nullptr, exception.what(), "Apex Small Fry Duo", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
}
