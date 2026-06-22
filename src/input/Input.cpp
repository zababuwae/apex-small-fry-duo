#include "Input.h"

void Input::BeginFrame()
{
    previousKeys_ = currentKeys_;
}

void Input::Reset()
{
    currentKeys_.fill(false);
    previousKeys_.fill(false);
}

void Input::SetKeyState(UINT virtualKey, bool isDown)
{
    if (IsValidKey(virtualKey))
    {
        currentKeys_[virtualKey] = isDown;
    }
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

bool Input::IsValidKey(UINT virtualKey) const
{
    return virtualKey < KeyCount;
}
