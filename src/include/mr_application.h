#ifndef _MR_APPLICATION_H_
#define _MR_APPLICATION_H_

#include "mr_window.h"

namespace mr
{
typedef void (*OnStartFn)();
typedef void (*OnUpdateFn)();
typedef void (*OnDestroyFn)();

class Application
{
public:
    typedef struct
    {
        OnStartFn onStart;
        OnUpdateFn onUpdate;
        OnDestroyFn onDestroy;
        Window::CreateParams windowCreateParams;
    } RunParams;

    typedef struct
    {
        Window::CreateParams windowCreateParams;
    } InitParams;

public:
    void Run(RunParams params);
    void Close();

private:
    Application();
    ~Application();

    bool Init(InitParams params);
    void Update();

private:
    bool running = false;
    Window *window = nullptr;

public:
    static Application &GetInstance()
    {
        static Application app = Application();
        return app;
    }
};
} // namespace mr

#endif