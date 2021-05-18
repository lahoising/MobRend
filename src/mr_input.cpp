#include "mr_input.h"

namespace mr
{
    
Input::Input()
{
    this->Clear();
}

void Input::Clear()
{
    this->currentState = {};
    this->prevState = {}; 
}

void Input::SwapState()
{
    this->prevState = this->currentState;
    this->currentState = {};
}

void Input::SubmitEvent(InputEvent event)
{
    switch (event.type)
    {
    case InputEventType::WINDOW_CLOSE:
        this->currentState.windowClose = event.event.windowClose;
        break;
    
    default:
        break;
    }
}

bool Input::WindowShouldClose()
{
    return this->currentState.windowClose.window;
}

} // namespace mr
