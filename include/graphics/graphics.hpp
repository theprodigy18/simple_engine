#pragma once

#include "common_header.hpp"
#include "platform/window.hpp"

#define USE_OPEN_GL 1

namespace drop::graphics
{
    DEFINE_TYPED_ID(contextID);
    

    struct ContextInitInfo
    {
        platform::WindowHandle handle;
        platform::WindowDC     dc;
    };


    bool      GraphicsInit();
    contextID CreateGraphicsContext(const ContextInitInfo* const initInfo);
    void      DestroyGraphicsContext(contextID id);
    void      GraphicsShutdown();
    void      UseContextFrame(contextID id);
} // namespace drop::graphics