#pragma once

#include "graphics/graphics.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#elif defined(__linux__)
#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif // _WIN32

#include "glcorearb.h"                

namespace drop::graphics
{

#ifdef _WIN32
#define LoadGLFunction(type, name)          \
    name = (type) wglGetProcAddress(#name); \
    SM_ASSERT(name, "Failed to load OpenGL function: %s" #name);

    using GLContext = HGLRC;
#elif defined(__linux__)
#define LoadGLFunction(type, name)          \
    name = (type) eglGetProcAddress(#name); \
    SM_ASSERT(name, "Failed to load OpenGL function: %s" #name);

    using GLContext = void*;
#endif // _WIN32

    using OpenGLInitFunctionPtr = void (*)();

    bool      OpenGLInit();
    contextID OpenGLCreateContext(const ContextInitInfo* const initInfo);
    void      OpenGLDestroyContext(contextID id);
    void      OpenGLShutdown();

    void BeginFrame(contextID id);
    void EndFrame(contextID id);
} // namespace drop::graphics