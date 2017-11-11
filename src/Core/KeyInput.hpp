//
// Created by mtesseract on 4-11-17.
//

#ifndef VULKANENGINE_KEYINPUT_H
#define VULKANENGINE_KEYINPUT_H

#include <GLFW/glfw3.h>
#include <map>

class KeyInput
{
public:
    void Initialize(GLFWwindow *p_window);
private:
    std::map<int, float> m_keys;
    void OnKeyAction(GLFWwindow * p_window, int p_key, int p_scancode, int p_action, int p_mods);
    void KeyUp(int p_key);
    void KeyDown(int p_key);
};


#endif //VULKANENGINE_KEYINPUT_H
