//
// Created by mtesseract on 17-1-18.
//

#ifndef VULKANENGINE_KEYINPUT_HPP
#define VULKANENGINE_KEYINPUT_HPP

#include <map>
#include "Timer.hpp"
#include <glfw/glfw3.h>

enum KeyCode
{
    Space        = GLFW_KEY_SPACE,
    Apostrophe   = GLFW_KEY_APOSTROPHE,
    Comma        = GLFW_KEY_COMMA,
    Minus        = GLFW_KEY_MINUS,
    Period       = GLFW_KEY_PERIOD,
    Slash        = GLFW_KEY_SLASH,
    Semicolon    = GLFW_KEY_SEMICOLON,
    Equal        = GLFW_KEY_EQUAL,
    Alpha0       = GLFW_KEY_0,
    Alpha1       = GLFW_KEY_1,
    Alpha2       = GLFW_KEY_2,
    Alpha3       = GLFW_KEY_3,
    Alpha4       = GLFW_KEY_4,
    Alpha5       = GLFW_KEY_5,
    Alpha6       = GLFW_KEY_6,
    Alpha7       = GLFW_KEY_7,
    Alpha8       = GLFW_KEY_8,
    Alpha9       = GLFW_KEY_9,
    A            = GLFW_KEY_A,
    B            = GLFW_KEY_B,
    C            = GLFW_KEY_C,
    D            = GLFW_KEY_D,
    E            = GLFW_KEY_E,
    F            = GLFW_KEY_F,
    G            = GLFW_KEY_G,
    H            = GLFW_KEY_H,
    I            = GLFW_KEY_I,
    J            = GLFW_KEY_J,
    K            = GLFW_KEY_K,
    L            = GLFW_KEY_L,
    M            = GLFW_KEY_M,
    N            = GLFW_KEY_N,
    O            = GLFW_KEY_O,
    P            = GLFW_KEY_P,
    Q            = GLFW_KEY_Q,
    R            = GLFW_KEY_R,
    S            = GLFW_KEY_S,
    T            = GLFW_KEY_T,
    U            = GLFW_KEY_U,
    V            = GLFW_KEY_V,
    W            = GLFW_KEY_W,
    X            = GLFW_KEY_X,
    Y            = GLFW_KEY_Y,
    Z            = GLFW_KEY_Z,
    LeftBracket  = GLFW_KEY_LEFT_BRACKET,
    RightBracket = GLFW_KEY_RIGHT_BRACKET,
    BackSlash    = GLFW_KEY_BACKSLASH,
    AccentGrave  = GLFW_KEY_GRAVE_ACCENT,
    World1       = GLFW_KEY_WORLD_1,
    World2       = GLFW_KEY_WORLD_2,
    Escape       = GLFW_KEY_ESCAPE,
    Enter        = GLFW_KEY_ENTER,
    Tab          = GLFW_KEY_TAB,
    Backspace    = GLFW_KEY_BACKSPACE,
    Insert       = GLFW_KEY_INSERT,
    Delete       = GLFW_KEY_DELETE,
    Right        = GLFW_KEY_RIGHT,
    Left         = GLFW_KEY_LEFT,
    Down         = GLFW_KEY_DOWN,
    Up           = GLFW_KEY_UP,
    PageUp       = GLFW_KEY_PAGE_UP,
    PageDown     = GLFW_KEY_PAGE_DOWN,
    Home         = GLFW_KEY_HOME,
    End          = GLFW_KEY_END,
    CapsLock     = GLFW_KEY_CAPS_LOCK,
    ScrollLock   = GLFW_KEY_SCROLL_LOCK,
    NumLock      = GLFW_KEY_NUM_LOCK,
    PrintScreen  = GLFW_KEY_PRINT_SCREEN,
    Pause        = GLFW_KEY_PAUSE,
    F1           = GLFW_KEY_F1,
    F2           = GLFW_KEY_F2,
    F3           = GLFW_KEY_F3,
    F4           = GLFW_KEY_F4,
    F5           = GLFW_KEY_F5,
    F6           = GLFW_KEY_F6,
    F7           = GLFW_KEY_F7,
    F8           = GLFW_KEY_F8,
    F9           = GLFW_KEY_F9,
    F10          = GLFW_KEY_F10,
    F11          = GLFW_KEY_F11,
    F12          = GLFW_KEY_F12,
    F13          = GLFW_KEY_F13,
    F14          = GLFW_KEY_F14,
    F15          = GLFW_KEY_F15,
    F16          = GLFW_KEY_F16,
    F17          = GLFW_KEY_F17,
    F18          = GLFW_KEY_F18,
    F19          = GLFW_KEY_F19,
    F20          = GLFW_KEY_F20,
    F21          = GLFW_KEY_F21,
    F22          = GLFW_KEY_F22,
    F23          = GLFW_KEY_F23,
    F24          = GLFW_KEY_F24,
    F25          = GLFW_KEY_F25,
    Keypad0      = GLFW_KEY_KP_0,
    Keypad1      = GLFW_KEY_KP_1,
    Keypad2      = GLFW_KEY_KP_2,
    Keypad3      = GLFW_KEY_KP_3,
    Keypad4      = GLFW_KEY_KP_4,
    Keypad5      = GLFW_KEY_KP_5,
    Keypad6      = GLFW_KEY_KP_6,
    Keypad7      = GLFW_KEY_KP_7,
    Keypad8      = GLFW_KEY_KP_8,
    Keypad9      = GLFW_KEY_KP_9,
    KeypadDec    = GLFW_KEY_KP_DECIMAL,
    KeypadDiv    = GLFW_KEY_KP_DIVIDE,
    KeypadMul    = GLFW_KEY_KP_MULTIPLY,
    KeypadSub    = GLFW_KEY_KP_SUBTRACT,
    KeypadAdd    = GLFW_KEY_KP_ADD,
    KeypadEnter  = GLFW_KEY_KP_ENTER,
    KeypadEqual  = GLFW_KEY_KP_EQUAL,
    Shift        = GLFW_KEY_LEFT_SHIFT,
    ControlLeft  = GLFW_KEY_LEFT_CONTROL,
    AltLeft      = GLFW_KEY_LEFT_ALT,
    SuperLeft    = GLFW_KEY_LEFT_SUPER,
    ShiftRight   = GLFW_KEY_RIGHT_SHIFT,
    ControlRight = GLFW_KEY_RIGHT_CONTROL,
    AltRight     = GLFW_KEY_RIGHT_ALT,
    SuperRight   = GLFW_KEY_RIGHT_SUPER,
    Menu         = GLFW_KEY_MENU,
    Last         = GLFW_KEY_LAST
};

class GLFWwindow;

class KeyInput
{
    static bool                 m_prepared;
    static std::map<int, float> m_keys;
    static Timer                m_timer;
    static double               m_timeNow;
    static double               m_timeStep;

    //GLFW callbacks
    static void KeyCallback(GLFWwindow *p_window, int p_key, int p_scancode, int p_action, int p_mod);

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
    static bool Enter(KeyCode p_key);

    static bool Pressed(KeyCode p_key);

};

#endif //VULKANENGINE_KEYINPUT_HPP
