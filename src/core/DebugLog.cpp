#include "DebugLog.h"

#if defined(_DEBUG)

#include <Windows.h>

#include <cstdarg>
#include <cstdio>

namespace
{
void WriteMessage(const char* level, const char* message)
{
    char output[1024]{};
    std::snprintf(output, sizeof(output), "[Apex][%s] %s\n", level, message);
    OutputDebugStringA(output);
}
}

void DebugLog::Info(const char* message)
{
    WriteMessage("Info", message);
}

void DebugLog::Warning(const char* message)
{
    WriteMessage("Warn", message);
}

void DebugLog::Error(const char* message)
{
    WriteMessage("Error", message);
}

void DebugLog::Format(const char* format, ...)
{
    char message[896]{};

    va_list arguments;
    va_start(arguments, format);
    std::vsnprintf(message, sizeof(message), format, arguments);
    va_end(arguments);

    WriteMessage("Info", message);
}

#endif
