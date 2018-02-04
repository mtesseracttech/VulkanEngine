//
// Created by mtesseract on 17-1-18.
//

#ifndef VULKANENGINE_KEYINPUT_HPP
#define VULKANENGINE_KEYINPUT_HPP

#include <map>
#include "Timer.hpp"

class GLFWwindow;

class KeyInput
{
    static bool m_prepared;
    static std::map<int, float> m_keys;
    static Timer m_timer;
    static double m_timeNow;
    static double m_timeStep;

    //GLFW callbacks
    static void KeyCallback(GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mod);

    //Key cache management functions
    static void KeyDown(int p_key);
    static void KeyUp(int p_key);

    //Technical functionality to be used by gamebase
    static void Initialize(GLFWwindow *p_window);
    static void UpdateTimeNow(double p_timeNow);

    //Gamebase is supposed to initialize this manager
    friend class GameBase;


public:
    //User Functionality
    static bool Enter(int p_key);
    static bool Pressed(int p_key);

};
#endif //VULKANENGINE_KEYINPUT_HPP
