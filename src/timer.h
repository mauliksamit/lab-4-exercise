#ifndef TIMER_H
#define TIMER_H

// replace with yours from the prep...
#include <chrono>
#include <cstdint>


class Timer
{
    public:

        using Nanos   = std::chrono::nanoseconds;
        using Micros  = std::chrono::microseconds;
        using Millis  = std::chrono::milliseconds;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours   = std::chrono::hours;
    
        Timer(): most_recent_checkpoint{std::chrono::steady_clock::now()}{};

        void restart()
        {
            most_recent_checkpoint = std::chrono::steady_clock::now();
        };

        template <typename T>
        uint64_t click()
        {
            auto new_time = std::chrono::steady_clock::now();
            uint64_t time_diff = std::chrono::duration_cast<T>(new_time - most_recent_checkpoint).count();
            most_recent_checkpoint = new_time;
            return time_diff;

        };

        template <typename T>
        uint64_t glance() const
        {

            auto new_time = std::chrono::steady_clock::now();
            uint64_t time_diff = std::chrono::duration_cast<T>(new_time - most_recent_checkpoint).count();
            return time_diff;
        };


    private:
       std::chrono::time_point<std::chrono::steady_clock> most_recent_checkpoint;



};
#endif  // TIMER_H
