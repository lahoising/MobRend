#ifndef _MR_WINDOW_H_
#define _MR_WINDOW_H_

#include <inttypes.h>
#include <unordered_map>
#include "mr_input.h"

namespace mr
{
    
class Window
{
public:
    typedef struct
    {
        uint32_t width, height;
        char *windowName;
    } CreateParams;

public:
    static Window *Create(CreateParams params);

    virtual ~Window() = 0;
    virtual void SwapBuffers() = 0;
    virtual void PollEvents() = 0;
    virtual void *GetHandle() = 0;
    virtual void SetCursorVisible(bool visible) = 0;
    virtual uint32_t GetWidth() = 0;
    virtual uint32_t GetHeight() = 0;
    virtual uint32_t GetFramebufferWidth() = 0;
    virtual uint32_t GetFramebufferHeight() = 0;

public:
    Input input;
};

class WindowManager
{
public:
    int GetWindowCount(){ return this->windowCount; }
    
    void AddWindow(Window *window) 
    { 
        this->windowCount++;
    }
    
    void RemoveWindow(Window *window) 
    { 
        this->windowCount--;
    }

public:
    static WindowManager &GetInstance()
    {
        static WindowManager manager = WindowManager();
        return manager;
    }

private:
    int windowCount;
    WindowManager(){ this->windowCount = 0; }
};

} // namespace mr

#endif