#ifdef WIN32
#include <windows.h>
#endif

#include "mr_window.h"

#if defined(_WIN32)
#define WINDOW_CLASS_NAME "Base Window Class"
#endif

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

Window::Window(char *windowName, int width, int height)
{
    WNDCLASS wc = { };

    this->hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = this->hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        WINDOW_CLASS_NAME,              // Window class
        windowName,    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return;
    }

    
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

Window::~Window()
{
    this->Destroy();
}

void Window::Show()
{
    
}

void Window::Destroy()
{
    
}

// ========================================

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);



            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}