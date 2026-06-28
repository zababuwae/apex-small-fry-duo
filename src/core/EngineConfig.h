#pragma once

#include <Windows.h>

#include <string>

struct EngineConfig final
{
    std::wstring windowTitle = L"Apex Small Fry Duo";
    UINT windowWidth = 1280;
    UINT windowHeight = 720;

    bool vSyncEnabled = true;
    unsigned int targetFrameRate = 60;
    double statusLogIntervalSeconds = 1.0;

    bool showCursor = true;
    bool lockCursorOnStart = true;
    UINT cursorLockToggleKey = VK_F1;
    UINT closeKey = VK_ESCAPE;

    float clearColorRed = 0.08f;
    float clearColorGreen = 0.12f;
    float clearColorBlue = 0.15f;
    float clearColorPulseAmount = 0.12f;
    float clearColorAlpha = 1.0f;
};
