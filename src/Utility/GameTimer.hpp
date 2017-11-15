//
// Created by MTesseracT on 15-11-2017.
//

#ifndef VULKANENGINE_GAMETIMER_HPP
#define VULKANENGINE_GAMETIMER_HPP

#include "Timer.hpp"

class GameTimer
{
public: //Typedefs
    typedef std::chrono::duration<double, std::ratio<1>> durationSec;
    typedef std::chrono::high_resolution_clock clock;

private:
    static std::chrono::time_point<clock> m_current;
    static std::chrono::time_point<clock> m_previous;
    static std::chrono::time_point<clock> m_start;
    static double m_deltaTime;

    friend class GameBase;

    static void Reset()
    {
        m_current   = clock::now();
        m_previous  = clock::now();
        m_start     = clock::now();
        m_deltaTime = 0.0;
    }
    static void UpdateDelta()
    {
        m_previous  = m_current;
        m_current   = clock::now();
        m_deltaTime = std::chrono::duration_cast<durationSec>(m_current - m_previous).count();
    }

public:
    static double Delta()
    {
        return m_deltaTime;
    }
    static double Current()
    {
        return std::chrono::duration_cast<durationSec>(m_current - m_start).count();
    }
    static double Real()
    {
        return std::chrono::duration_cast<durationSec>(clock::now() - m_start).count();
    }
};

std::chrono::time_point<GameTimer::clock> GameTimer::m_current  = clock::now();
std::chrono::time_point<GameTimer::clock> GameTimer::m_previous = clock::now();
std::chrono::time_point<GameTimer::clock> GameTimer::m_start    = clock::now();
double GameTimer::m_deltaTime = 0.0;

#endif //VULKANENGINE_GAMETIMER_HPP
