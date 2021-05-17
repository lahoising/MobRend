#ifndef _MR_WINDOW_H_
#define _MR_WINDOW_H_

#include "unordered_map"
#include "mr_input.h"

namespace mr
{

typedef void* window_handle;
    
class Window
{
public:
    typedef struct
    {
        int width, height;
        char *windowName;
    } CreateParams;

public:
    virtual ~Window() = 0 {};
    virtual void SwapBuffers() = 0;
    virtual window_handle GetHandle() = 0;
    virtual void Close() = 0;

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
        // this->windowMap[window->GetHandle()] = window;
    }
    
    void RemoveWindow(Window *window) 
    { 
        this->windowCount--;
        // window_handle handle = window->GetHandle();
        // if(this->windowMap.find(handle) != this->windowMap.end())
        //     this->windowMap.erase(window->GetHandle());
    }
    
    Window *GetWindowByHandle(window_handle handle)
    { 
        return this->windowMap[handle]; 
    }

public:
    static WindowManager &GetInstance()
    {
        static WindowManager manager = WindowManager();
        return manager;
    }

private:
    int windowCount;
    std::unordered_map<window_handle,Window*> windowMap;
    WindowManager(){ this->windowCount = 0; }
};

} // namespace mr

#endif