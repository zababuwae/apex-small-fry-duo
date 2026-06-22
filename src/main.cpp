#include "core/Application.h"
#include "core/DebugLog.h"

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
        DebugLog::Info("Application starting.");
        Application application(instance, showCommand);
        const int exitCode = application.Run();
        DebugLog::Format("Application stopped. Code=%d", exitCode);
        return exitCode;
    }
    catch (const std::exception& exception)
    {
        DebugLog::Error(exception.what());
        MessageBoxA(nullptr, exception.what(), "Apex Small Fry Duo", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
}
