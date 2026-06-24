#pragma once

#include <Windows.h>

#include <array>
#include <cstddef>

class Input final
{
public:
    struct MousePosition final
    {
        int x = 0;
        int y = 0;
    };

    void BeginFrame();
    void Reset();
    void SetKeyState(UINT virtualKey, bool isDown);
    void SetMousePosition(int x, int y);
    void SetLeftMouseButtonState(bool isDown);

    bool IsKeyDown(UINT virtualKey) const;
    bool WasKeyPressed(UINT virtualKey) const;
    bool WasKeyReleased(UINT virtualKey) const;
    MousePosition GetMousePosition() const;
    bool IsLeftMouseButtonDown() const;
    bool WasLeftMouseButtonPressed() const;
    bool WasLeftMouseButtonReleased() const;

private:
    static constexpr std::size_t KeyCount = 256;

    bool IsValidKey(UINT virtualKey) const;

    std::array<bool, KeyCount> currentKeys_{};
    std::array<bool, KeyCount> previousKeys_{};
    MousePosition mousePosition_{};
    bool currentLeftMouseButton_ = false;
    bool previousLeftMouseButton_ = false;
};
