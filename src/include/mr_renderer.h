#ifndef _MR_RENDERER_H_
#define _MR_RENDERER_H_

namespace mr
{

class Mesh;
class Model;

enum TopologyType
{
    TRIANGLES = 0,
    WIREFRAME,
};

enum RenderObjectType
{
    NONE,
    MESH,
    MODEL
};

class Renderer
{
public:

    struct gui_init_info_s;
    typedef struct command_s
    {
        union
        {
            Mesh *mesh;
            Model *model;
        };
        TopologyType topologyType;
        RenderObjectType renderObjectType;

        struct command_s()
            : mesh(nullptr), model(nullptr), 
            topologyType(TopologyType::TRIANGLES),
            renderObjectType(RenderObjectType::NONE)
            {}
    } Command;

public:
    static Renderer *Create();
    virtual ~Renderer() = 0 {};
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void OnRenderBegin() = 0;
    virtual void OnRenderEnd() = 0;
    virtual gui_init_info_s *GetGuiInitInfo() = 0;
    virtual void DeleteGuiInitInfo(gui_init_info_s *info) = 0;
    virtual void Render(Command &cmd) = 0;
};
    
} // namespace mr


#endif