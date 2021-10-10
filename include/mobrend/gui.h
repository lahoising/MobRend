#ifndef _MR_GUI_H_
#define _MR_GUI_H_

#include "mr_renderer.h"

namespace mr
{
    
class Gui
{
public:
    typedef struct
    {
        void *windowHandle;
        struct Renderer::gui_init_info_s *rendererInitInfo;
    } CreateParams;

public:
    Gui(CreateParams params);
    ~Gui();

    void BeginFrame();
    void EndFrame();
};

} // namespace mr

#endif