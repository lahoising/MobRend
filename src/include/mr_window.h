#ifndef _MR_WINDOW_H_
#define _MR_WINDOW_H_

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
};

#endif