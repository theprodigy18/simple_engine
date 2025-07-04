#pragma once

#include "common_header.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif // _WIN32

// Use later when we refactor the window type to flags based.
// #define WINDOW_TYPE_NORMAL 0x00000001
// #define WINDOW_TYPE_CHILD  0x00000002
// #define WINDOW_TYPE_NORESIZE 0x00000004
// #define WINDOW_TYPE_NOMINIMIZE 0x00000008
// #define WINDOW_TYPE_NOCLOSE 0x00000010
// #define WINDOW_TYPE_NOBORDER 0x00000020

namespace drop::platform
{
    DEFINE_TYPED_ID(windowID);

#ifdef _WIN32
    using TITLE        = const WChar;
    using WindowProc   = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);
    using WindowHandle = HWND;
    using WindowDC     = HDC;
#elif defined(__linux__)
    using TITLE        = const Char;
    using WindowProc   = void (*)(const XEvent&, windowID);
    using WindowHandle = ::Window;
    using WindowDC     = Display*;
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
    WindowDC     GetWindowDC(windowID id);
    void         Update(bool& running);
    void         SetGraphicsContext(windowID id, id::idType contextID);

#ifdef __linux__
    // Display* GetDisplay();
#endif // __linux__
} // namespace drop::platform
