#ifndef _MR_INPUT_H_
#define _MR_INPUT_H_

#include "mr_input_event.h"

namespace mr
{

typedef struct
{
    WindowCloseEvent windowClose;
    KeyEvent keys[512];
} InputState;

class Input
{
public:
    Input();
    void SwapState();
    void Clear();

    bool WindowShouldClose();
    
    bool IsKeyPressed(int key);
    bool KeyJustPressed(int key);
    bool KeyJustReleased(int key);
    
    void SubmitEvent(InputEvent event);

private:
    InputState currentState;
    InputState prevState;
};

} // namespace mr


#endif