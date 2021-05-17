#ifndef _MR_INPUT_H_
#define _MR_INPUT_H_

#include "mr_input_event.h"

namespace mr
{

typedef struct
{
    WindowCloseEvent windowClose;
} InputState;

class Input
{
public:
    Input();
    void SwapState();
    void Clear();

    bool WindowShouldClose();
    
    void SubmitEvent(InputEvent event);

private:
    InputState currentState;
    InputState prevState;
};

} // namespace mr


#endif