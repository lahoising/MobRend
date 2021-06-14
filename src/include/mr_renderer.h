#ifndef _MR_RENDERER_H_
#define _MR_RENDERER_H_

#include <inttypes.h>

namespace mr
{

class Mesh;
class Model;

enum TopologyType
{
    TOPOLOGY_TRIANGLES = 0,
    TOPOLOGY_WIREFRAME,
};

enum RenderObjectType
{
    RENDER_OBJECT_NONE = 0,
    RENDER_OBJECT_MESH,
    RENDER_OBJECT_MODEL
};

enum RenderPass
{
    RENDER_PASS_NONE = 0,
    RENDER_PASS_DEPTH = 1,
    RENDER_PASS_STENCIL = 2
};

enum RenderPassFn
{
    RENDER_PASS_FN_ALWAYS,
    RENDER_PASS_FN_NEVER,
    RENDER_PASS_FN_LESS,
    RENDER_PASS_FN_EQUAL,
    RENDER_PASS_FN_LESS_OR_EQUEAL,
    RENDER_PASS_FN_GREATER,
    RENDER_PASS_FN_NOT_EQUAL,
    RENDER_PASS_FN_GREATER_OR_EQUAL,
};

enum StencilAction
{
    STENCIL_ACTION_KEEP,
    STENCIL_ACTION_ZERO,
    STENCIL_ACTION_REPLACE,
    STENCIL_ACTION_INCREASE,
    STENCIL_ACTION_INCREASE_WRAP,
    STENCIL_ACTION_DECREASE,
    STENCIL_ACTION_DECREASE_WRAP,
    STENCIL_ACTION_INVERT,
};

typedef unsigned int RenderPassMask;

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
            topologyType(TopologyType::TOPOLOGY_TRIANGLES),
            renderObjectType(RenderObjectType::RENDER_OBJECT_NONE)
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

    virtual void EnableRenderPass(RenderPassMask renderPassMask, bool enable) = 0;
    virtual void SetDepthTestFn(RenderPassFn fn) = 0;
    virtual void SetStencilTestFn(RenderPassFn fn, int refValue, unsigned int mask) = 0;
    virtual void SetStencilTestAction(
        StencilAction stencilFailAction, 
        StencilAction depthFailAction, 
        StencilAction stencilAndDepthFailAction) = 0;
    virtual void SetStencilMask(uint8_t mask) = 0;
};
    
} // namespace mr


#endif