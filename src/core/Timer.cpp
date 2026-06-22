#include "Timer.h"

#include <algorithm>

void Timer::Reset()
{
    startTime_ = Clock::now();
    previousTime_ = startTime_;
    deltaTime_ = 0.0;
    totalTime_ = 0.0;
}

void Timer::Tick()
{
    const Clock::time_point currentTime = Clock::now();
    const std::chrono::duration<double> frameDuration = currentTime - previousTime_;
    const std::chrono::duration<double> elapsedDuration = currentTime - startTime_;

    deltaTime_ = std::min(frameDuration.count(), 0.1);
    totalTime_ = elapsedDuration.count();
    previousTime_ = currentTime;
}

double Timer::GetDeltaTime() const
{
    return deltaTime_;
}

double Timer::GetTotalTime() const
{
    return totalTime_;
}
