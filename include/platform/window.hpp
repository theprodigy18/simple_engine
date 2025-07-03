#pragma once

#include "common_header.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif // _WIN32

namespace drop::platform
{
    DEFINE_TYPED_ID(windowID);
    
#ifdef _WIN32
    using TITLE        = const WChar;
    using WindowProc   = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);
    using WindowHandle = HWND;
#elif defined(__linux__)
    using TITLE        = const Char;
    using WindowProc   = void (*)(const XEvent&, windowID);
    using WindowHandle = ::Window;
#endif // _WIN32

    enum WindowType : u8
    {
        WINDOW_TYPE_NORMAL,
        WINDOW_TYPE_CHILD,
        WINDOW_TYPE_NORESIZE,

        WINDOW_TYPE_COUNT
    };

    struct WindowInitInfo
    {
        WindowHandle parent;
        WindowProc   callback {nullptr};
        TITLE        title;
        i32          width {1280};
        i32          height {720};
        WindowType   type {WINDOW_TYPE_NORMAL};
    };

    windowID     CreatePlatformWindow(const WindowInitInfo* const initInfo);
    void         DestroyPlatformWindow(windowID id);
    void         Cleanup();
    WindowHandle GetWindowHandle(windowID id);
    void         Update(bool& running);

#ifdef __linux__
    // Display* GetDisplay();    
#endif // __linux__
} // namespace drop::platform
