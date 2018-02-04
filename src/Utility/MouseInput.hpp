//
// Created by mtesseract on 25-1-18.
//

#ifndef VULKANENGINE_MOUSEINPUT_HPP
#define VULKANENGINE_MOUSEINPUT_HPP


#include <map>
#include <glm/vec2.hpp>
#include "Timer.hpp"

class GLFWwindow;

class MouseInput
{
    static bool m_prepared;
    static std::map<int, float> m_buttons;
    static Timer m_timer;
    static double m_timeNow;
    static double m_timeStep;
    static glm::vec2 m_position;

    //GLFW callbacks
    static void MouseButtonCallback(GLFWwindow * p_window, int p_button, int p_action, int p_mods);
    static void MousePositionCallback(GLFWwindow * p_window, double p_xPos, double p_yPos);

    //Button Cache management functions
    static void ButtonUp(int p_button);
    static void ButtonDown(int p_button);

    //Technical functionality to be used by gamebase
    static void Initialize(GLFWwindow * p_window);
    static void UpdateTimeNow(double p_timeNow);

    //Gamebase is supposed to initialize this manager
    friend class GameBase;

public:
    //User Functionality
    static bool Enter(int p_button);
    static bool Pressed(int p_button);
    static glm::vec2 Position();

};


#endif //VULKANENGINE_MOUSEINPUT_HPP
