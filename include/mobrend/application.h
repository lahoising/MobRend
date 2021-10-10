#ifndef _MR_APPLICATION_H_
#define _MR_APPLICATION_H_

#include "mr_window.h"
#include "mr_renderer.h"
#include "mr_gui.h"

namespace mr
{
class Program
{
public:
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender(Renderer *renderer) = 0;
    virtual void OnGuiRender() = 0;
    virtual void OnDestroy() = 0;
};

class Application
{
public:
    typedef struct
    {
        Program *program;
        Window::CreateParams windowCreateParams;
    } RunParams;

private:
    typedef struct
    {
        Window::CreateParams windowCreateParams;
    } InitParams;

public:
    void Run(RunParams params);
    void Close();

    Window *GetMainWindow(){ return this->window; }
    Renderer *GetRenderer(){ return this->renderer; }
    Gui *GetGuiFramework(){ return this->gui; }

private:
    Application();
    ~Application();

    bool Init(InitParams params);
    void Update();
    void Render(Program *prog);

private:
    bool running = false;
    Window *window = nullptr;
    Renderer *renderer = nullptr;
    Gui *gui = nullptr;

public:
    static Application &GetInstance()
    {
        static Application app = Application();
        return app;
    }
};
} // namespace mr

#endif
