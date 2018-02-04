//
// Created by mtesseract on 25-1-18.
//

#include "GameTimer.hpp"

std::chrono::time_point<GameTimer::clock> GameTimer::m_current  = clock::now();
std::chrono::time_point<GameTimer::clock> GameTimer::m_previous = clock::now();
std::chrono::time_point<GameTimer::clock> GameTimer::m_start    = clock::now();
double GameTimer::m_deltaTime = 0.0;

void GameTimer::Reset()
{
    m_current   = clock::now();
    m_previous  = clock::now();
    m_start     = clock::now();
    m_deltaTime = 0.0;
}

void GameTimer::Update()
{
    m_previous  = m_current;
    m_current   = clock::now();
    m_deltaTime = std::chrono::duration_cast<durationSec>(m_current - m_previous).count();
}

double GameTimer::Delta()
{
    return m_deltaTime;
}

double GameTimer::Current()
{
    return std::chrono::duration_cast<durationSec>(m_current - m_start).count();
}

double GameTimer::Real()
{
    return std::chrono::duration_cast<durationSec>(clock::now() - m_start).count();
}