#pragma once

namespace DebugLog
{
#if defined(_DEBUG)
void Info(const char* message);
void Warning(const char* message);
void Error(const char* message);
void Format(const char* format, ...);
#else
inline void Info(const char*)
{
}

inline void Warning(const char*)
{
}

inline void Error(const char*)
{
}

inline void Format(const char*, ...)
{
}
#endif
}
