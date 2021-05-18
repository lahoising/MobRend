#ifndef _MR_INPUT_EVENT_H_
#define _MR_INPUT_EVENT_H_

namespace mr
{
class Window;

enum InputEventType
{
    UNKNOWN = 0,
    KEY_EVENT,
    WINDOW_CLOSE,
};

typedef struct
{
    int key;
    int pressed;
    int mod;
} KeyEvent;

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
        WindowCloseEvent windowClose;
    };
} InputEvent;

} // namespace mr

#endif