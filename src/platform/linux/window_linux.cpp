#include "platform/window.hpp"

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <unordered_map>

namespace drop::platform
{
    namespace
    {
        struct WindowInfo
        {
            ::Window              window {0};
            ::Window              parent {0};
            utl::vector<windowID> children;
            GC                    context {nullptr};
            WindowProc            callback {nullptr};
            XRectangle            rc {0, 0, 0, 0};
            WindowType            type {WINDOW_TYPE_NORMAL};
            bool                  isAlive {false};
        };

        utl::vector<WindowInfo>                windows;
        utl::vector<id::generationType>        generations;
        utl::deque<windowID>                   freeIds;
        std::unordered_map<::Window, windowID> windowMap;

        i32 activeWindowCount {0};

        Display* display {XOpenDisplay(nullptr)};
        Atom     wmDeleteMessageAtom {XInternAtom(display, "WM_DELETE_WINDOW", False)};
        Atom     wmStateAtom {XInternAtom(display, "_NET_WM_STATE", False)};
        Atom     wmMaxHorzAtom {XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False)};
        Atom     wmMaxVertAtom {XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False)};
        Atom     wmTypeAtom {XInternAtom(display, "_NET_WM_WINDOW_TYPE", False)};
        Atom     wmDialogAtom {XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False)};

        bool IsAlive(windowID id)
        {
            windowID index {id::GetIndex(id)};
            return windows[index].isAlive && id::GetGeneration(id) == generations[index];
        }

        void GetCenteredPosition(i32 screen, i32 width, i32 height, i32& outX, i32& outY, const WindowInfo* parentInfo = nullptr)
        {
            Screen* screenInfo {XScreenOfDisplay(display, screen)};
            i32     screenWidth {screenInfo->width};
            i32     screenHeight {screenInfo->height};

            if (parentInfo)
            {
                const i32 px {parentInfo->rc.x};
                const i32 py {parentInfo->rc.y};
                const i32 pw {parentInfo->rc.width};
                const i32 ph {parentInfo->rc.height};

                const i32 centerX {px + pw / 2 - width / 2};
                const i32 centerY {py + ph / 2 - height / 2};

                const bool isFullyVisible {
                    (centerX >= 0 && centerX + width <= screenWidth) &&
                    (centerY >= 0 && centerY + height <= screenHeight)};

                if (isFullyVisible)
                {
                    outX = centerX;
                    outY = centerY;
                    return;
                }
            }

            // Fallback: center on screen
            outX = (screenWidth - width) / 2;
            outY = (screenHeight - height) / 2;
        }

        void ApplyWindowTypeHints(::Window window, const WindowInitInfo* const info)
        {
            switch (info->type)
            {
            case WINDOW_TYPE_NORESIZE:
            {
                XSizeHints* sizeHints {XAllocSizeHints()};
                if (sizeHints)
                {
                    sizeHints->flags      = PMinSize | PMaxSize | PSize | USSize;
                    sizeHints->min_width  = info->width;
                    sizeHints->min_height = info->height;
                    sizeHints->max_width  = info->width;
                    sizeHints->max_height = info->height;
                    sizeHints->width      = info->width;
                    sizeHints->height     = info->height;

                    XSetWMNormalHints(display, window, sizeHints);
                    XFree(sizeHints);
                }
            }
            case WINDOW_TYPE_CHILD:
            {
                XChangeProperty(display, window, wmTypeAtom, XA_ATOM, 32, PropModeReplace, (u8*) &wmDialogAtom, 1);
                XSetTransientForHint(display, window, info->parent);
                break;
            }
            }
        }

        bool HandleNORESIZEEvent(const XEvent& event, WindowInfo& info, windowID id)
        {
            switch (event.type)
            {
            case ConfigureNotify:
            {
                const XConfigureEvent& config {event.xconfigure};
                info.rc.x = config.x;
                info.rc.y = config.y;
                if (config.width != info.rc.width || config.height != info.rc.height)
                {
                    XResizeWindow(config.display, config.window, info.rc.width, info.rc.height);
                    return true;
                }
                break;
            }
            case PropertyNotify:
            {
                const XPropertyEvent& prop {event.xproperty};
                if (prop.atom == wmStateAtom)
                {

                    XEvent xev {};
                    xev.xclient.type         = ClientMessage;
                    xev.xclient.serial       = 0;
                    xev.xclient.send_event   = True;
                    xev.xclient.window       = prop.window;
                    xev.xclient.message_type = wmStateAtom;
                    xev.xclient.format       = 32;
                    xev.xclient.data.l[0]    = 0; // 1 = add, 0 = remove
                    xev.xclient.data.l[1]    = wmMaxHorzAtom;
                    xev.xclient.data.l[2]    = wmMaxVertAtom;
                    xev.xclient.data.l[3]    = 0;

                    // We need to send event to WM so we can cancel maximize.
                    XSendEvent(display, DefaultRootWindow(display), False,
                               SubstructureRedirectMask | SubstructureNotifyMask, &xev);

                    XFlush(display); // Force flush.
                    const XConfigureEvent& config {event.xconfigure};
                    info.rc.x = config.x;
                    info.rc.y = config.y;
                    XMoveResizeWindow(prop.display, prop.window, info.rc.x, info.rc.y, info.rc.width, info.rc.height);
                    return true;
                }
                break;
            }
            default:
                break;
            }

            return false;
        }

        bool HandleInternalEvent(const XEvent& event, WindowInfo& info, windowID id)
        {
            if (info.type == WINDOW_TYPE_NORESIZE)
            {
                bool handled {HandleNORESIZEEvent(event, info, id)};
                if (handled)
                {
                    return true;
                }
            }

            switch (event.type)
            {
            case ClientMessage:
            {
                if ((Atom) event.xclient.data.l[0] == wmDeleteMessageAtom && IsAlive(id))
                {
                    if (!info.children.empty())
                    {
                        for (windowID child : info.children)
                        {
                            // SM_ASSERT(IsAlive(child), "Child window is not alive.");
                            WindowInfo& childInfo {windows[id::GetIndex(child)]};
                            childInfo.parent = 0;
                            DestroyPlatformWindow(child);
                            SM_TRACE("Destroyed child window: %d", child);
                        }

                        info.children.clear();
                    }
                    DestroyPlatformWindow(id);
                    return true;
                }
                break;
            }
            case DestroyNotify:
            {
                break;
            }
            default:
                break;
            }

            return false;
        }

        void DispatchEvent(const XEvent& event)
        {
            auto it {windowMap.find(event.xany.window)};
            if (it == windowMap.end())
                return;

            windowID    id {it->second};
            WindowInfo& info {windows[id::GetIndex(id)]};

            if (!info.isAlive)
            {
                return;
            }

            bool handled {HandleInternalEvent(event, info, id)};
            if (!handled)
                info.callback(event, id);
        }

    } // namespace anonymous

    windowID CreatePlatformWindow(const WindowInitInfo* const initInfo)
    {
        WindowInfo info {};

        ::Window root {XDefaultRootWindow(display)};
        i32      defaultScreen {XDefaultScreen(display)};
        info.context = XDefaultGC(display, defaultScreen);

        u32                  attribValueMask {CWBackPixel | CWEventMask};
        XSetWindowAttributes attribs {};
        attribs.background_pixel = 0xff323232;
        attribs.event_mask       = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ExposureMask | PropertyChangeMask;

        i32         x {0}, y {0};
        WindowInfo* parentInfo {nullptr};
        if (initInfo->parent)
        {
            windowID parentID {windowMap.find(initInfo->parent)->second};
            SM_ASSERT(IsAlive(parentID), "Parent window is not alive.");
            parentInfo = &windows[id::GetIndex(parentID)];
        }
        GetCenteredPosition(defaultScreen, initInfo->width, initInfo->height, x, y, parentInfo);

        info.window = XCreateWindow(
            display,
            root,
            x, y,
            initInfo->width, initInfo->height,
            1,
            CopyFromParent,
            InputOutput,
            CopyFromParent,
            attribValueMask,
            &attribs);
        SM_ASSERT(info.window, "Failed to create window.");
        SM_TRACE("Created window: %d", info.window);

        XStoreName(display, info.window, initInfo->title);
        ApplyWindowTypeHints(info.window, initInfo);
        XSetWMProtocols(display, info.window, &wmDeleteMessageAtom, 1);
        info.rc       = XRectangle {(i16) x, (i16) y, (u16) initInfo->width, (u16) initInfo->height};
        info.isAlive  = true;
        info.callback = initInfo->callback;
        info.type     = initInfo->type;
        info.parent   = initInfo->parent;

        XMapWindow(display, info.window);
        XFlush(display);

        windowID id {id::invalidID};
        if (freeIds.size() <= id::minDeletedElements)
        {
            id = windowID {(id::idType) windows.size()};
            SM_ASSERT(id::IsValid(id), "Failed to get window id.");
            windows.push_back(info);
            generations.push_back(0);
        }
        else
        {
            id = freeIds.front();
            SM_ASSERT(!id::IsValid(id), "This id still in use.");
            freeIds.pop_front();
            id                        = windowID {id::NewGeneration(id)};
            windows[id::GetIndex(id)] = info;
            generations[id::GetIndex(id)]++;
        }

        windowMap[info.window] = id;

        if (info.parent)
        {
            windowID parentID {windowMap.find(info.parent)->second};
            SM_ASSERT(IsAlive(parentID), "Parent window is not alive.");
            WindowInfo& parentInfo {windows[id::GetIndex(parentID)]};
            parentInfo.children.push_back(id);
        }

        activeWindowCount++;
        SM_TRACE("activeWindowCount: %d", activeWindowCount);
        return id;
    }

    void DestroyPlatformWindow(windowID id)
    {
        windowID index {id::GetIndex(id)};
        SM_ASSERT(index < windows.size(), "Invalid index in windowID.");
        SM_ASSERT(id::IsValid(id), "Invalid windowID.");
        SM_ASSERT(IsAlive(id), "Invalid generation in windowID or window is not alive. %d", id);
        WindowInfo& info {windows[index]};

        if (info.parent)
        {
            windowID parentID {windowMap.find(info.parent)->second};
            SM_ASSERT(IsAlive(parentID), "Parent window is not alive.");
            WindowInfo& parentInfo {windows[id::GetIndex(parentID)]};
            auto        it {std::remove(parentInfo.children.begin(), parentInfo.children.end(), id)};
            parentInfo.children.erase(it, parentInfo.children.end());
        }

        info.isAlive = false;
        windowMap.erase(info.window);
        XUnmapWindow(display, info.window);
        XDestroyWindow(display, info.window);
        // XFreeGC(info->display, info->context); // Only free context if we created manually (in future).
        info.context  = nullptr;
        info.window   = 0;
        info.callback = nullptr;
        freeIds.push_back(id);
        activeWindowCount--;
        SM_TRACE("activeWindowCount: %d", activeWindowCount);
        SM_TRACE("Destroyed window: %d", id);
    }

    void Cleanup()
    {
        XCloseDisplay(display);
        display = nullptr;
    }

    WindowHandle GetWindowHandle(windowID id)
    {
        return windows[id::GetIndex(id)].window;
    }

    void Update(bool& running)
    {
        XEvent event {};
        while (XPending(display))
        {
            XNextEvent(display, &event);
            DispatchEvent(event);
        }

        if (activeWindowCount <= 0)
        {
            running = false;
        }
    }

    Display* GetDisplay()
    {
        return display;
    }
} // namespace drop::platform