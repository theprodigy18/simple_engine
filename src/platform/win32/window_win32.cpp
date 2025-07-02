#include "platform/window.hpp"

namespace drop::platform
{
    namespace
    {
        struct WindowInfo
        {
            HWND hwnd {nullptr};
            HDC  hdc {nullptr};
            RECT rc {0, 0, 0, 0};
            bool isAlive {false};
        };

        utl::vector<WindowInfo>         windows;
        utl::vector<id::generationType> generations;
        utl::deque<windowID>            freeIds;

        i32 activeWindowCount {0};

        using WindowFlags = u32;
        WindowFlags registeredClasses {0};

        constexpr TITLE GetWindowClassName(WindowType type)
        {
            switch (type)
            {
            case WINDOW_TYPE_NORMAL:
                return TITLE(L"WINDOW_TYPE_NORMAL");
            case WINDOW_TYPE_CHILD:
                return TITLE(L"WINDOW_TYPE_CHILD");
            case WINDOW_TYPE_NORESIZE:
                return TITLE(L"WINDOW_TYPE_NORESIZE");
            default:
                return nullptr;
            }
        }
        constexpr u32 GetWindowStyle(WindowType type)
        {
            switch (type)
            {
            case WINDOW_TYPE_NORMAL:
                return WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
            case WINDOW_TYPE_CHILD:
                return WS_POPUP | WS_CAPTION | WS_SYSMENU;
            case WINDOW_TYPE_NORESIZE:
                return WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            default:
                return 0;
            }
        }

        bool IsAlive(windowID id)
        {
            windowID index {id::GetIndex(id)};
            return windows[index].isAlive && id::GetGeneration(id) == generations[index];
        }

        WindowInfo GetWindowInfoFromID(windowID id)
        {
            SM_ASSERT(id::IsValid(id), "Invalid windowID.");
            SM_ASSERT(IsAlive(id), "Invalid generation in windowID or window is not alive.");
            return windows[id::GetIndex(id)];
        }
        WindowInfo GetWindowInfoFromHandle(HWND hwnd)
        {
            windowID id {(id::idType) GetWindowLongPtrW(hwnd, GWLP_USERDATA)};
            return GetWindowInfoFromID(id);
        }

        LRESULT CALLBACK InternalWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            WindowInfo* info {nullptr};
            switch (msg)
            {
            case WM_CLOSE:
            {
                windowID id {(id::idType) GetWindowLongPtrW(hWnd, GWLP_USERDATA)};
                SM_TRACE("WM_DESTROY: %d", id);
                SM_TRACE("activeWindowCount: %d", activeWindowCount);
                SM_TRACE("hwnd: %d", hWnd);
                DestroyPlatformWindow(id);

                if (activeWindowCount <= 0)
                {
                    SM_TRACE("count: %d", activeWindowCount);
                    PostQuitMessage(0);
                }
                return 0;
            }
            case WM_DESTROY:
            {
                windowID id {(id::idType) GetWindowLongPtrW(hWnd, GWLP_USERDATA)};
                if (IsAlive(id))
                {
                    DestroyPlatformWindow(id);
                }
                if (activeWindowCount <= 0)
                {
                    SM_TRACE("count: %d", activeWindowCount);
                    PostQuitMessage(0);
                }
                return 0;
            }
            default:
                break;
            }

            LONG_PTR longPtr {GetWindowLongPtrW(hWnd, 0)};
            return longPtr ? ((WindowProc) longPtr)(hWnd, msg, wParam, lParam) : DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    } // namespace anonymous

    windowID CreatePlatformWindow(const WindowInitInfo* const initInfo)
    {
        HINSTANCE hInstance {GetModuleHandleW(nullptr)};
        SM_ASSERT(hInstance, "Failed to get module handle.");

        TITLE className {GetWindowClassName(initInfo->type)};
        SM_ASSERT(className, "Failed to get window class name.");

        bool isClassRegistered = registeredClasses & (1 << static_cast<u8>(initInfo->type));
        if (!isClassRegistered)
        {

            // Create the window class and fill out the WNDCLASSEXW structure.
            WNDCLASSEXW wcex {};
            ZeroMemory(&wcex, sizeof(WNDCLASSEXW)); // Just make sure it's zeroed.
            wcex.cbSize        = sizeof(WNDCLASSEXW);
            wcex.style         = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc   = InternalWndProc;
            wcex.cbClsExtra    = 0;
            wcex.cbWndExtra    = sizeof(initInfo->callback);
            wcex.hInstance     = hInstance;
            wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
            wcex.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
            wcex.hIconSm       = LoadIcon(hInstance, IDI_APPLICATION);
            wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
            wcex.lpszClassName = className;
            wcex.lpszMenuName  = nullptr;

            SM_ASSERT(RegisterClassExW(&wcex), "Failed to register window class.");
            registeredClasses |= 1 << (static_cast<u8>(initInfo->type));
        }

        u32 dwStyle {GetWindowStyle(initInfo->type)};

        WindowInfo info {};
        RECT       rc {0, 0, initInfo->width, initInfo->height};
        AdjustWindowRect(&rc, dwStyle, false);

        i32 x {0}, y {0};

        // Centering the window base on their parent (if doesn't have parent then center on screen).
        if (initInfo->type == WINDOW_TYPE_CHILD && initInfo->parent)
        {
            RECT parentRect {};
            GetWindowRect(initInfo->parent, &parentRect); // full window, including title bar.

            i32 parentWidth {parentRect.right - parentRect.left};
            i32 parentHeight {parentRect.bottom - parentRect.top};

            i32 windowWidth {rc.right - rc.left};
            i32 windowHeight {rc.bottom - rc.top};

            x = parentRect.left + (parentWidth - windowWidth) / 2;
            y = parentRect.top + (parentHeight - windowHeight) / 2;
        }
        else
        {
            RECT dr {};
            SystemParametersInfoW(SPI_GETWORKAREA, 0, &dr, 0);

            i32 dw {dr.right - dr.left};
            i32 dh {dr.bottom - dr.top};

            i32 ww {rc.right - rc.left};
            i32 wh {rc.bottom - rc.top};

            x = dr.left + (dw - ww) / 2;
            y = dr.top + (dh - wh) / 2;
        }

        info.hwnd = CreateWindowExW(
            0,
            className,
            initInfo->title,
            dwStyle,
            x,
            y,
            rc.right - rc.left,
            rc.bottom - rc.top,
            initInfo->parent,
            nullptr,
            hInstance,
            nullptr);

        SM_ASSERT(info.hwnd, "Failed to create window.");

        info.rc = rc;

        info.hdc = GetDC(info.hwnd);
        SM_ASSERT(info.hdc, "Failed to get device context.");
        info.isAlive = true;

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

        SetWindowLongPtrW(info.hwnd, GWLP_USERDATA, (LONG_PTR) id);
        SetWindowLongPtrW(info.hwnd, 0, (LONG_PTR) initInfo->callback);

        ShowWindow(info.hwnd, SW_SHOW);
        // UpdateWindow(info.hwnd);
        activeWindowCount++;

        return id;
    }

    void DestroyPlatformWindow(windowID id)
    {
        windowID index {id::GetIndex(id)};
        SM_ASSERT(index < windows.size(), "Invalid index in windowID.");
        SM_ASSERT(id::IsValid(id), "Invalid windowID.");
        SM_ASSERT(IsAlive(id), "Invalid generation in windowID or window is not alive.");
        WindowInfo* info {&windows[index]};
        SM_ASSERT(info->isAlive, "Window is not alive.");

        info->isAlive = false;
        ReleaseDC(info->hwnd, info->hdc);
        DestroyWindow(info->hwnd);
        freeIds.push_back(id);
        activeWindowCount--;
    }

    void Cleanup()
    {
        HINSTANCE hInstance {GetModuleHandleW(nullptr)};

        UnregisterClassW(GetWindowClassName(WINDOW_TYPE_NORMAL), hInstance);
        UnregisterClassW(GetWindowClassName(WINDOW_TYPE_CHILD), hInstance);
        UnregisterClassW(GetWindowClassName(WINDOW_TYPE_NORESIZE), hInstance);
    }

    WindowHandle GetWindowHandle(windowID id)
    {
        return GetWindowInfoFromID(id).hwnd;
    }

} // namespace drop::platform