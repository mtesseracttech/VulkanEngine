//
// Created by mtesseract on 18-1-18.
//

#include <GLFW/glfw3.h>
#include "KeyInput.hpp"

bool                 KeyInput::m_prepared = false;
std::map<int, float> KeyInput::m_keys;
Timer                KeyInput::m_timer;
double               KeyInput::m_timeNow;
double               KeyInput::m_timeStep = 1.0 / 60;

//Call before doing anything with the class
void KeyInput::Initialize(GLFWwindow *p_window)
{
    if (m_prepared) return;

    glfwSetKeyCallback(p_window, KeyInput::KeyCallback);

    m_timer.Reset();

    m_prepared = true;
}

void KeyInput::KeyCallback(GLFWwindow *p_window, int p_key, int p_scancode, int p_action, int p_mod)
{
    switch (p_action)
    {
        case GLFW_PRESS:
            KeyDown(p_key);
            break;
        case GLFW_RELEASE:
            KeyUp(p_key);
            break;
        default:
            break;
    }
}


//Key State Management
void KeyInput::KeyUp(int p_key)
{
    m_keys[p_key] = 0.0f;
}

void KeyInput::KeyDown(int p_key)
{
    auto it = m_keys.find(p_key);

    if (it != m_keys.end() && m_keys[p_key] == 0.0f)
    {
        m_keys[p_key] = static_cast<float>(m_timer.GetElapsed());
    }
}

//Public Accessible Functions
bool KeyInput::Enter(int p_key)
{
    auto it = m_keys.find(p_key);

    if (it != m_keys.end())
    {
        return m_keys[p_key] > m_timeNow - m_timeStep;
    }
    return false;
}

bool KeyInput::Pressed(int p_key)
{
    auto it = m_keys.find(p_key);

    if (it != m_keys.end())
    {
        return m_keys[p_key] > 0.0f;
    }
    return false;
}

void KeyInput::UpdateTimeNow(double p_timeNow)
{
    m_timeNow = p_timeNow;
}

