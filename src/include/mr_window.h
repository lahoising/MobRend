#ifndef _MR_WINDOW_H_
#define _MR_WINDOW_H_

#if defined _WIN32
#include <windows.h>
#endif

class Window
{
public:
    Window(char *windowName, int width, int height);
    ~Window();

    void Show();
    void Destroy();

private:
    char *windowName;
    int width ;
    int height;

    #if defined(_WIN32)
    HINSTANCE hInstance;
    HWND window;
    #endif
};

#endif