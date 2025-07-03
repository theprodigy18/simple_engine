#include "platform/window.hpp"

using namespace drop;

static bool s_Running {false};

#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

platform::WindowProc   callback {WndProc};
platform::WindowHandle defaultHandle {nullptr};

#elif defined(__linux__)
void Callback(const XEvent& event, platform::windowID id) { }

platform::WindowProc   callback {Callback};
platform::WindowHandle defaultHandle {0};
#endif // _WIN32

int main()
{
#ifdef _WIN32
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
    SetProcessDPIAware();
    platform::TITLE title1 {platform::TITLE(L"Normal Window")};
    platform::TITLE title2 {platform::TITLE(L"Child Window")};
    platform::TITLE title3 {platform::TITLE(L"NORESIZE Window")};
#elif defined(__linux__)
    platform::TITLE title1 {platform::TITLE("Normal Window")};
    platform::TITLE title2 {platform::TITLE("Child Window")};
    platform::TITLE title3 {platform::TITLE("NORESIZE Window")};
#endif // _WIN32

    // SM_ASSERT(false, "test");

    platform::WindowInitInfo info1 {defaultHandle, callback, title1, 1280, 720, platform::WINDOW_TYPE_NORMAL};

    platform::windowID       id {platform::CreatePlatformWindow(&info1)};
    platform::WindowHandle   parent {platform::GetWindowHandle(id)};
    platform::WindowInitInfo info2 {parent, callback, title2, 640, 480, platform::WINDOW_TYPE_CHILD};
    platform::CreatePlatformWindow(&info2);

    platform::WindowInitInfo info3 {defaultHandle, callback, title3, 1440, 768, platform::WINDOW_TYPE_NORESIZE};
    platform::CreatePlatformWindow(&info3);

    s_Running = true;
    while (s_Running)
    {
        platform::Update(s_Running);
    }

    platform::Cleanup();

    // int* leaks = new int[64];

    return 0;
}