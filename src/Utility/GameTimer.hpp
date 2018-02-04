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

    static void Reset();
    static void Update();

    friend class GameBase;

public:
    static double Delta();
    static double Current();
    static double Real();
};
#endif //VULKANENGINE_GAMETIMER_HPP
