#include <memory>
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
    memcpy(
        this->currentState.keys,
        this->prevState.keys,
        sizeof(KeyEvent) * INPUT_STATE_KEY_EVENT_NUM
    );
}

void Input::SubmitEvent(InputEvent event)
{
    switch (event.type)
    {
    case InputEventType::WINDOW_CLOSE:
        this->currentState.windowClose = event.windowClose;
        break;

    case InputEventType::KEY_EVENT:
        this->currentState.keys[event.keyEvent.key] = event.keyEvent;
        break;
    
    default:
        break;
    }
}

bool Input::WindowShouldClose()
{
    return this->currentState.windowClose.window;
}

bool Input::IsKeyPressed(int key)
{
    return this->currentState.keys[key].pressed;
}

bool Input::KeyJustPressed(int key)
{
    return this->currentState.keys[key].pressed && !this->prevState.keys[key].pressed;
}

bool Input::KeyJustReleased(int key)
{
    return !this->currentState.keys[key].pressed && this->prevState.keys[key].pressed;
}


} // namespace mr
