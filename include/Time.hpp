#ifndef FENNTON_TIME_HPP
#define FENNTON_TIME_HPP

#include <chrono>

namespace Fennton::Time {
    using Ms = std::chrono::milliseconds;
    using Us = std::chrono::microseconds;
    using Seconds = std::chrono::seconds;
    using FloatDuration = std::chrono::duration<float>;

    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using TimeDuration = std::chrono::high_resolution_clock::duration;

    class Timer {
        // The timer's current time.
        TimePoint time = TimePoint(Seconds(0));
        // The accumulator.
        TimeDuration accumulator = TimeDuration(0);
        // The update rate. By default it's one second.
        TimeDuration rate = Seconds(1);
        // The maximum update rate, to prevent too much updates in a single call of loop.
        // By default it's one second.
        TimeDuration maxRate = Seconds(1);
        // The alpha, a value used for interpolation.
        float alpha = 1.0f;
    public:
        Timer(TimePoint time, TimeDuration rate, TimeDuration maxRate);
        Timer() = default;
        Timer(Timer const& other) = default;
        Timer(Timer&& other) = default;
        Timer& operator=(Timer const& other) = default;
        Timer& operator=(Timer&& other) = default;

        // Updates the timer with the specified delta time and calls f for each complete cycle.
        void loop(TimeDuration delta, auto f) {
            // Updates the accumulator.
            accumulator += delta;
            // Limits how many steady updates can be executed in a single frame.
            accumulator = min(accumulator, maxRate);

            while (accumulator >= rate) {
                // Updates the steady time.
                time += rate;
                // Updates the accumulator.
                accumulator -= rate;
                // Executes the callback once for the current cycle.
                f();
            }

            alpha = FloatDuration(accumulator).count() / FloatDuration(rate).count();
        }
        // Sets the timer's time.
        void SetTime(TimePoint newTime);
        // Sets the timer's accumulator.
        void SetAccumulator(TimeDuration newAccumulator);
        // Sets the timer's update rate.
        void SetRate(TimeDuration newRate);
        // Sets the timer's maximum update rate.
        void SetMaxRate(TimeDuration newMaxRate);
        // Returns the timer's time.
        TimePoint GetTime() const;
        // Returns the timer's accumulator.
        TimeDuration GetAccumulator() const;
        // Returns the timer's update rate.
        TimeDuration GetRate() const;
        // Returns the timer's maximum update rate.
        TimeDuration GetMaxRate() const;
    };

    // Initialises the time.
    void init();
    // Updates the time.
    void update();
    // Returns the exact current time.
    TimePoint getNow() noexcept;
    // Returns the time since the last call to Time::update.
    TimePoint getTime() noexcept;
    // Returns the delta time, the interval between the last update and the previous one.
    TimeDuration getDeltaTime() noexcept;
    // Returns the alpha value calculated at the last tick, used for interpolation.
    /* float getAlpha() noexcept;
    void setTickRate() noexcept;
    TimeDuration getTickRate() noexcept;
    void setMaxTickRate() noexcept;
    TimeDuration getMaxTickRate() noexcept; */
}
#endif