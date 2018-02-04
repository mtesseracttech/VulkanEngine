//
// Created by mtesseract on 25-1-18.
//

#include <GLFW/glfw3.h>
#include "MouseInput.hpp"

bool                 MouseInput::m_prepared = false;
std::map<int, float> MouseInput::m_buttons;
Timer                MouseInput::m_timer;
double               MouseInput::m_timeNow;
double               MouseInput::m_timeStep = 1.0 / 60;
glm::vec2            MouseInput::m_position;

//Call before doing anything with the class
void MouseInput::Initialize(GLFWwindow *p_window)
{
    if(m_prepared) return;

    glfwSetMouseButtonCallback(p_window, MouseInput::MouseButtonCallback);
    glfwSetCursorPosCallback(p_window, MouseInput::MousePositionCallback);

    m_timer.Reset();

    m_prepared = true;
}

//Callbacks
void MouseInput::MouseButtonCallback(GLFWwindow *p_window, int p_button, int p_action, int p_mods)
{
    switch (p_action){
        case GLFW_PRESS:
            ButtonDown(p_button);
            break;
        case GLFW_RELEASE:
            ButtonUp(p_button);
            break;
        default:
            break;
    }
}

void MouseInput::MousePositionCallback(GLFWwindow *p_window, double p_xPos, double p_yPos)
{
    m_position = glm::vec2(p_xPos, p_yPos);
}

//Button State Management
void MouseInput::ButtonUp(int p_button)
{
    m_buttons[p_button] = 0.0f;
}

void MouseInput::ButtonDown(int p_button)
{
    auto it = m_buttons.find(p_button);

    if (it != m_buttons.end() && m_buttons[p_button] == 0.0f)
    {
        m_buttons[p_button] = static_cast<float>(m_timer.GetElapsed());
    }
}

//Public accessible data
glm::vec2 MouseInput::Position()
{
    return m_position;
}

bool MouseInput::Enter(int p_button)
{
    auto it = m_buttons.find(p_button);

    if (it != m_buttons.end())
    {
        return m_buttons[p_button] > m_timeNow - m_timeStep;
    }
    return false;
}

bool MouseInput::Pressed(int p_button)
{
    auto it = m_buttons.find(p_button);

    if (it != m_buttons.end())
    {
        return m_buttons[p_button] > 0.0f;
    }
    return false;
}

void MouseInput::UpdateTimeNow(double p_timeNow)
{
    m_timeNow = p_timeNow;
}
