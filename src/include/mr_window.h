#ifndef _MR_WINDOW_H_
#define _MR_WINDOW_H_

#include <unordered_map>
#include "mr_input.h"

namespace mr
{
    
class Window
{
public:
    typedef struct
    {
        int width, height;
        char *windowName;
    } CreateParams;

public:
    static Window *Create(CreateParams params);

    virtual ~Window() = 0 {};
    virtual void SwapBuffers() = 0;

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