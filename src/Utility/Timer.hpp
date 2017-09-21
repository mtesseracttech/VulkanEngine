//
// Created by MTesseracT on 20-9-2017.
//

#ifndef VULKANENGINE_TIMER_HPP
#define VULKANENGINE_TIMER_HPP

#include <iostream>
#include <chrono>
#include <cstdlib>

class Timer
{
public:
    Timer() : m_start(std::chrono::high_resolution_clock::now()) {}
    void Reset() { m_start = chronoClock::now(); }
    double GetElapsed() const
    {
        return std::chrono::duration_cast<chronoSecond>(chronoClock::now() - m_start).count();
    }

private:
    typedef std::chrono::high_resolution_clock chronoClock;
    typedef std::chrono::duration<double, std::ratio<1> > chronoSecond;
    std::chrono::time_point<chronoClock> m_start;
};


#endif //VULKANENGINE_TIMER_HPP
