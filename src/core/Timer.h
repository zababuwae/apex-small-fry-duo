#pragma once

#include <chrono>

class Timer final
{
public:
    void Reset();
    void Tick();

    double GetDeltaTime() const;
    double GetTotalTime() const;

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point startTime_ = Clock::now();
    Clock::time_point previousTime_ = startTime_;
    double deltaTime_ = 0.0;
    double totalTime_ = 0.0;
};
