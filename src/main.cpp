#include "platform/window.hpp"

#ifdef _DEBUG && defined(_WIN32)
#include <crtdbg.h>
#endif // _DEBUG

// #include <Windows.h>
using namespace drop;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

platform::WindowProc callback {WndProc};
static bool          s_Running {false};

int main()
{
#ifdef _WIN32
    SetProcessDPIAware();
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
#endif // _WIN32

	//SM_ASSERT(false, "test");

    platform::WindowInitInfo info1 {nullptr, callback, platform::TITLE(L"Main Window"), 1280, 720, platform::WINDOW_TYPE_NORMAL};

    platform::windowID     id {platform::CreatePlatformWindow(&info1)};
    platform::WindowHandle parent {platform::GetWindowHandle(id)};
    platform::WindowInitInfo info2 {parent, callback, platform::TITLE(L"Child Window"), 640, 480, platform::WINDOW_TYPE_CHILD};
    platform::CreatePlatformWindow(&info2);
	platform::CreatePlatformWindow(&info2);
	platform::WindowInitInfo info3 {nullptr, callback, platform::TITLE(L"Hub"), 1366, 768, platform::WINDOW_TYPE_NORESIZE};
	platform::CreatePlatformWindow(&info3);

    s_Running = true;
    while (s_Running)
    {
        MSG msg {};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);

            if (msg.message == WM_QUIT)
            {
                s_Running = false;
            }
        }

    }

    platform::Cleanup();

	//int* leaks = new int[64];

    return 0;
}