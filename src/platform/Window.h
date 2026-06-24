#pragma once

#include <Windows.h>

#include <functional>
#include <string>

class Window final
{
public:
    using ResizeCallback = std::function<void(UINT, UINT)>;
    using KeyCallback = std::function<void(UINT, bool)>;
    using MouseMoveCallback = std::function<void(int, int)>;
    using LeftMouseButtonCallback = std::function<void(bool, int, int)>;
    using FocusLostCallback = std::function<void()>;

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
    void RequestClose() const;
    void SetCursorVisible(bool isVisible) const;
    void LockCursorToClient();
    void UnlockCursor();
    void SetResizeCallback(ResizeCallback callback);
    void SetKeyCallback(KeyCallback callback);
    void SetMouseMoveCallback(MouseMoveCallback callback);
    void SetLeftMouseButtonCallback(LeftMouseButtonCallback callback);
    void SetFocusLostCallback(FocusLostCallback callback);

private:
    static LRESULT CALLBACK WindowProcedure(
        HWND windowHandle,
        UINT message,
        WPARAM wordParameter,
        LPARAM longParameter);

    LRESULT HandleMessage(UINT message, WPARAM wordParameter, LPARAM longParameter);
    void UpdateCursorClip() const;

    HINSTANCE instance_ = nullptr;
    HWND handle_ = nullptr;
    std::wstring className_;
    UINT clientWidth_ = 0;
    UINT clientHeight_ = 0;
    bool isCursorLocked_ = false;
    ResizeCallback resizeCallback_;
    KeyCallback keyCallback_;
    MouseMoveCallback mouseMoveCallback_;
    LeftMouseButtonCallback leftMouseButtonCallback_;
    FocusLostCallback focusLostCallback_;
};
