#include <Time.hpp>
#include <Application.hpp>

namespace Fennton::Time {
    static TimePoint currTime, lastTime/* , tickTime */;
    static TimeDuration deltaTime/* , accumulator, tickRate, maxTickRate */;
    /* static float alpha; */

    void Timer::SetTime(TimePoint newTime) {
        time = newTime;
    }
    void Timer::SetAccumulator(TimeDuration newAccumulator) {
        accumulator = newAccumulator;
    }
    void Timer::SetRate(TimeDuration newRate) {
        rate = newRate;
    }
    void Timer::SetMaxRate(TimeDuration newMaxRate) {
        maxRate = newMaxRate;
    }
    TimePoint Timer::GetTime() const {
        return time;
    }
    TimeDuration Timer::GetAccumulator() const {
        return accumulator;
    }
    TimeDuration Timer::GetRate() const {
        return rate;
    }
    TimeDuration Timer::GetMaxRate() const {
        return maxRate;
    }

    void init() {
        currTime = getNow();
        /* tickTime = currTime; */
    }
    void update() {
        lastTime = currTime;
        currTime = getNow();
        // Calculates how much time the last frame took.
        deltaTime = currTime - lastTime;
    }
    TimePoint getNow() noexcept {
        return std::chrono::high_resolution_clock::now();
    }
    TimePoint getTime() noexcept {
        return currTime;
    }
    TimeDuration getDeltaTime() noexcept {
        return deltaTime;
    }
    /* float getAlpha() noexcept {
        return alpha;
    } */
    /* void setTickRate() noexcept;
    TimeDuration getTickRate() noexcept;
    void setMaxTickRate() noexcept;
    TimeDuration getMaxTickRate() noexcept; */
}