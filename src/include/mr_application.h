#ifndef _MR_APPLICATION_H_
#define _MR_APPLICATION_H_

namespace mr
{
typedef void (*OnStartFn)();
typedef void (*OnUpdateFn)();
typedef void (*OnDestroyFn)();

class Window;

class Application
{
public:
    typedef struct
    {
        OnStartFn onStart;
        OnUpdateFn onUpdate;
        OnDestroyFn onDestroy;
    } RunParams;

public:
    void Run(RunParams params);
    void Close();

private:
    Application();
    ~Application();

    bool Init();
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