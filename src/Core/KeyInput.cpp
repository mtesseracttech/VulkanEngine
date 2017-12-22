//
// Created by mtesseract on 4-11-17.
//

#include <Utility/Logger.hpp>
#include "KeyInput.hpp"

std::map<int, double> KeyInput::m_keys;

Timer KeyInput::m_timer;

void KeyInput::Initialize(GLFWwindow * p_window)
{
    glfwSetKeyCallback(p_window, KeyInput::OnKeyAction);
    KeyInput::m_timer.Reset();
}

void KeyInput::OnKeyAction(GLFWwindow *p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
    switch (p_action)
    {
        case GLFW_PRESS:
            KeyInput::KeyUp(p_key);
            break;
        case GLFW_RELEASE:
            KeyInput::KeyDown(p_key);
            break;
        default:
            return;
    }
}

void KeyInput::KeyUp(int p_key)
{
    m_keys[p_key] = 0.0;
}

void KeyInput::KeyDown(int p_key)
{
    double time = m_keys[p_key];
    if(time == 0.0)
    {
        m_keys[p_key] = m_timer.GetElapsed();
    }
}

bool KeyInput::GetKeyDown(int p_key)
{
    return m_keys[p_key] > m_timer.GetElapsed() - 1.0/60.0;
}

bool KeyInput::GetKey(int p_key)
{
    try
    {
        return m_keys.at(p_key) > 0.0f;
    }
    catch (const std::out_of_range& ex)
    {
        return false;
    }
}
