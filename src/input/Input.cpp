#include "Input.h"

void Input::BeginFrame()
{
    previousKeys_ = currentKeys_;
    previousLeftMouseButton_ = currentLeftMouseButton_;
}

void Input::Reset()
{
    currentKeys_.fill(false);
    previousKeys_.fill(false);
    currentLeftMouseButton_ = false;
    previousLeftMouseButton_ = false;
}

void Input::SetKeyState(UINT virtualKey, bool isDown)
{
    if (IsValidKey(virtualKey))
    {
        currentKeys_[virtualKey] = isDown;
    }
}

void Input::SetMousePosition(int x, int y)
{
    mousePosition_.x = x;
    mousePosition_.y = y;
}

void Input::SetLeftMouseButtonState(bool isDown)
{
    currentLeftMouseButton_ = isDown;
}

bool Input::IsKeyDown(UINT virtualKey) const
{
    return IsValidKey(virtualKey) && currentKeys_[virtualKey];
}

bool Input::WasKeyPressed(UINT virtualKey) const
{
    return IsValidKey(virtualKey)
        && currentKeys_[virtualKey]
        && !previousKeys_[virtualKey];
}

bool Input::WasKeyReleased(UINT virtualKey) const
{
    return IsValidKey(virtualKey)
        && !currentKeys_[virtualKey]
        && previousKeys_[virtualKey];
}

Input::MousePosition Input::GetMousePosition() const
{
    return mousePosition_;
}

bool Input::IsLeftMouseButtonDown() const
{
    return currentLeftMouseButton_;
}

bool Input::WasLeftMouseButtonPressed() const
{
    return currentLeftMouseButton_ && !previousLeftMouseButton_;
}

bool Input::WasLeftMouseButtonReleased() const
{
    return !currentLeftMouseButton_ && previousLeftMouseButton_;
}

bool Input::IsValidKey(UINT virtualKey) const
{
    return virtualKey < KeyCount;
}
