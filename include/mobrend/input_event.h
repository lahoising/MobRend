#ifndef _MR_INPUT_EVENT_H_
#define _MR_INPUT_EVENT_H_

namespace mr
{
class Window;

enum InputEventType
{
    UNKNOWN = 0,
    KEY_EVENT,
    MOUSE_POSITION,
    WINDOW_CLOSE
};

enum KeyCode
{
    KEY_UNKNOWN = 0,
    KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
    KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L,
    KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,
    KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_DASH, KEY_BACKSLASH, KEY_EQUALS,
    KEY_LBRACKET, KEY_RBRACKET,
    KEY_ESC, KEY_APOSTROPHE,
    KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
    KEY_SEMICOLON, KEY_GRAVE,
    KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
    KEY_SPACE, KEY_BACKSPACE,
    KEY_HOME, KEY_END, KEY_PAGE_UP, KEY_PAGE_DOWN,
    KEY_CAPS_LOCK, KEY_PRINT_SCREEN, KEY_DELETE, KEY_INSERT,
    KEY_NUM_LOCK,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9,
    KEY_F10, KEY_F11, KEY_F12, KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19,
    KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24, KEY_F25,
    KEY_TAB,
    KEY_LCTRL, KEY_LSHIFT, KEY_LALT, KEY_LSUPER,
    KEY_RCTRL, KEY_RSHIFT, KEY_RALT, KEY_RSUPER,
};

typedef struct
{
    KeyCode key;
    int pressed;
    int mod;
} KeyEvent;

typedef struct
{
    float x, y;
} MousePositionEvent;

typedef struct
{
    Window *window;
} WindowCloseEvent;

typedef struct
{
    InputEventType type;
    union
    {
        KeyEvent keyEvent;
        MousePositionEvent mousePosition;
        WindowCloseEvent windowClose;
    };
} InputEvent;

} // namespace mr

#endif