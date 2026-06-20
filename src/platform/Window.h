#pragma once

#include <Windows.h>

#include <functional>
#include <string>

class Window final
{
public:
    using ResizeCallback = std::function<void(UINT, UINT)>;

    Window(
        HINSTANCE instance,
        int showCommand,
        const std::wstring& title,
        UINT clientWidth,
        UINT clientHeight);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ProcessMessages() const;
    HWND GetHandle() const;
    UINT GetClientWidth() const;
    UINT GetClientHeight() const;
    void SetResizeCallback(ResizeCallback callback);

private:
    static LRESULT CALLBACK WindowProcedure(
        HWND windowHandle,
        UINT message,
        WPARAM wordParameter,
        LPARAM longParameter);

    LRESULT HandleMessage(UINT message, WPARAM wordParameter, LPARAM longParameter);

    HINSTANCE instance_ = nullptr;
    HWND handle_ = nullptr;
    std::wstring className_;
    UINT clientWidth_ = 0;
    UINT clientHeight_ = 0;
    ResizeCallback resizeCallback_;
};
