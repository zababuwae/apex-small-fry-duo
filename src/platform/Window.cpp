#include "Window.h"
#include "../core/DebugLog.h"

#include <stdexcept>
#include <utility>

Window::Window(
    HINSTANCE instance,
    int showCommand,
    const std::wstring& title,
    UINT clientWidth,
    UINT clientHeight)
    : instance_(instance),
      className_(L"ApexSmallFryDuoWindowClass"),
      clientWidth_(clientWidth),
      clientHeight_(clientHeight)
{
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance_;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.lpszClassName = className_.c_str();

    if (RegisterClassExW(&windowClass) == 0)
    {
        throw std::runtime_error("Failed to register the window class.");
    }

    RECT windowRectangle{0, 0, static_cast<LONG>(clientWidth_), static_cast<LONG>(clientHeight_)};
    if (AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE) == FALSE)
    {
        throw std::runtime_error("Failed to calculate the window size.");
    }

    handle_ = CreateWindowExW(
        0,
        className_.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRectangle.right - windowRectangle.left,
        windowRectangle.bottom - windowRectangle.top,
        nullptr,
        nullptr,
        instance_,
        this);

    if (handle_ == nullptr)
    {
        UnregisterClassW(className_.c_str(), instance_);
        throw std::runtime_error("Failed to create the window.");
    }

    ShowWindow(handle_, showCommand);
    UpdateWindow(handle_);
    DebugLog::Format("Window created. Size=%ux%u", clientWidth_, clientHeight_);
}

Window::~Window()
{
    if (handle_ != nullptr)
    {
        DestroyWindow(handle_);
    }

    if (instance_ != nullptr && !className_.empty())
    {
        UnregisterClassW(className_.c_str(), instance_);
    }
}

bool Window::ProcessMessages() const
{
    MSG message{};

    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE) != FALSE)
    {
        if (message.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return true;
}

HWND Window::GetHandle() const
{
    return handle_;
}

UINT Window::GetClientWidth() const
{
    return clientWidth_;
}

UINT Window::GetClientHeight() const
{
    return clientHeight_;
}

void Window::RequestClose() const
{
    if (handle_ != nullptr)
    {
        PostMessageW(handle_, WM_CLOSE, 0, 0);
    }
}

void Window::SetResizeCallback(ResizeCallback callback)
{
    resizeCallback_ = std::move(callback);
}

void Window::SetKeyCallback(KeyCallback callback)
{
    keyCallback_ = std::move(callback);
}

void Window::SetFocusLostCallback(FocusLostCallback callback)
{
    focusLostCallback_ = std::move(callback);
}

LRESULT CALLBACK Window::WindowProcedure(
    HWND windowHandle,
    UINT message,
    WPARAM wordParameter,
    LPARAM longParameter)
{
    Window* window = nullptr;

    if (message == WM_NCCREATE)
    {
        const auto* createStructure = reinterpret_cast<CREATESTRUCTW*>(longParameter);
        window = static_cast<Window*>(createStructure->lpCreateParams);
        SetWindowLongPtrW(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->handle_ = windowHandle;
    }
    else
    {
        window = reinterpret_cast<Window*>(GetWindowLongPtrW(windowHandle, GWLP_USERDATA));
    }

    if (window != nullptr)
    {
        return window->HandleMessage(message, wordParameter, longParameter);
    }

    return DefWindowProcW(windowHandle, message, wordParameter, longParameter);
}

LRESULT Window::HandleMessage(UINT message, WPARAM wordParameter, LPARAM longParameter)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (keyCallback_)
        {
            keyCallback_(static_cast<UINT>(wordParameter), true);
        }
        return 0;

    case WM_KEYUP:
        if (keyCallback_)
        {
            keyCallback_(static_cast<UINT>(wordParameter), false);
        }
        return 0;

    case WM_KILLFOCUS:
        if (focusLostCallback_)
        {
            focusLostCallback_();
        }
        return 0;

    case WM_SIZE:
        clientWidth_ = LOWORD(longParameter);
        clientHeight_ = HIWORD(longParameter);

        if (wordParameter != SIZE_MINIMIZED && resizeCallback_)
        {
            DebugLog::Format("Window resized. Size=%ux%u", clientWidth_, clientHeight_);
            resizeCallback_(clientWidth_, clientHeight_);
        }
        return 0;

    case WM_CLOSE:
        DebugLog::Info("Window close requested.");
        DestroyWindow(handle_);
        return 0;

    case WM_DESTROY:
        DebugLog::Info("Window destroyed.");
        handle_ = nullptr;
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(handle_, message, wordParameter, longParameter);
    }
}
