//
// Created by mtesseract on 4-11-17.
//

#include "KeyInput.h"

void KeyInput::Initialize(GLFWwindow * p_window)
{
    glfwSetKeyCallback(p_window, OnKeyAction);
}

void KeyInput::OnKeyAction(GLFWwindow *p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
    switch (p_action)
    {
        case GLFW_PRESS:
            KeyUp(p_key);
            break;
        case GLFW_RELEASE:
            KeyDown(p_key);
            break;
        default:
            return;
    }
}

void KeyInput::KeyUp(int p_key)
{
    m_keys[p_key] = 0.0f;
}

void KeyInput::KeyDown(int p_key)
{
    float time = m_keys[p_key];
    if(time == 0.0f)
    {
        
    }
}
