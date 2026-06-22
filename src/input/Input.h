#pragma once

#include <Windows.h>

#include <array>
#include <cstddef>

class Input final
{
public:
    void BeginFrame();
    void Reset();
    void SetKeyState(UINT virtualKey, bool isDown);

    bool IsKeyDown(UINT virtualKey) const;
    bool WasKeyPressed(UINT virtualKey) const;
    bool WasKeyReleased(UINT virtualKey) const;

private:
    static constexpr std::size_t KeyCount = 256;

    bool IsValidKey(UINT virtualKey) const;

    std::array<bool, KeyCount> currentKeys_{};
    std::array<bool, KeyCount> previousKeys_{};
};
