//
// Created by mtesseract on 4-11-17.
//

#ifndef VULKANENGINE_KEYINPUT_H
#define VULKANENGINE_KEYINPUT_H

#include <GLFW/glfw3.h>
#include <map>
#include <Utility/Timer.hpp>

class KeyInput
{
public:
    static void Initialize(GLFWwindow* p_window);
    static bool GetKeyDown(int p_key);
    static bool GetKey(int p_key);
private:
    static std::map<int, double> m_keys;
    static Timer                m_timer;

    static void OnKeyAction(GLFWwindow * p_window, int p_key, int p_scancode, int p_action, int p_mods);
    static void KeyUp(int p_key);
    static void KeyDown(int p_key);
};


#endif //VULKANENGINE_KEYINPUT_H
