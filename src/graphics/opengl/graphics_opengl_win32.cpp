#include "graphics/opengl/graphics_opengl.hpp"

#include "wglext.h"
#include <gl/GL.h>

namespace drop::graphics
{
    namespace
    {
        HMODULE openglDLL {nullptr};

#define LoadGLFunction(type, name)                  \
    name = (type) wglGetProcAddress(#name);         \
    if (!name)                                      \
    {                                               \
    name = (type) GetProcAddress(openglDLL, #name); \
    }                                               \
    SM_ASSERT(name, "Failed to load OpenGL function: %s" #name);

#pragma region GLFunctionProcs
        static PFNGLCREATEPROGRAMPROC           glCreateProgram {nullptr};
        static PFNGLDELETETEXTURESPROC          glDeleteTextures {nullptr};
        static PFNGLGENTEXTURESPROC             glGenTextures {nullptr};
        static PFNGLBINDTEXTUREPROC             glBindTexture {nullptr};
        static PFNGLDRAWBUFFERPROC              glDrawBuffer {nullptr};
        static PFNGLDRAWARRAYSPROC              glDrawArrays {nullptr};
        static PFNGLCREATESHADERPROC            glCreateShader {nullptr};
        static PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation {nullptr};
        static PFNGLUNIFORM1FPROC               glUniform1f {nullptr};
        static PFNGLUNIFORM2FVPROC              glUniform2fv {nullptr};
        static PFNGLUNIFORM3FVPROC              glUniform3fv {nullptr};
        static PFNGLUNIFORM1IPROC               glUniform1i {nullptr};
        static PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv {nullptr};
        static PFNGLVERTEXATTRIBDIVISORPROC     glVertexAttribDivisor {nullptr};
        static PFNGLACTIVETEXTUREPROC           glActiveTexture {nullptr};
        static PFNGLBUFFERSUBDATAPROC           glBufferSubData {nullptr};
        static PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced {nullptr};
        static PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer {nullptr};
        static PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus {nullptr};
        static PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers {nullptr};
        static PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D {nullptr};
        static PFNGLDRAWBUFFERSPROC             glDrawBuffers {nullptr};
        static PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers {nullptr};
        static PFNGLBLENDFUNCIPROC              glBlendFunci {nullptr};
        static PFNGLBLENDEQUATIONPROC           glBlendEquation {nullptr};
        static PFNGLCLEARBUFFERFVPROC           glClearBufferfv {nullptr};
        static PFNGLSHADERSOURCEPROC            glShaderSource {nullptr};
        static PFNGLCOMPILESHADERPROC           glCompileShader {nullptr};
        static PFNGLGETSHADERIVPROC             glGetShaderiv {nullptr};
        static PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog {nullptr};
        static PFNGLATTACHSHADERPROC            glAttachShader {nullptr};
        static PFNGLLINKPROGRAMPROC             glLinkProgram {nullptr};
        static PFNGLVALIDATEPROGRAMPROC         glValidateProgram {nullptr};
        static PFNGLGETPROGRAMIVPROC            glGetProgramiv {nullptr};
        static PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog {nullptr};
        static PFNGLGENBUFFERSPROC              glGenBuffers {nullptr};
        static PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays {nullptr};
        static PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation {nullptr};
        static PFNGLBINDVERTEXARRAYPROC         glBindVertexArray {nullptr};
        static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray {nullptr};
        static PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer {nullptr};
        static PFNGLBINDBUFFERPROC              glBindBuffer {nullptr};
        static PFNGLBINDBUFFERBASEPROC          glBindBufferBase {nullptr};
        static PFNGLBUFFERDATAPROC              glBufferData {nullptr};
        static PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv {nullptr};
        static PFNGLUSEPROGRAMPROC              glUseProgram {nullptr};
        static PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays {nullptr};
        static PFNGLDELETEBUFFERSPROC           glDeleteBuffers {nullptr};
        static PFNGLDELETEPROGRAMPROC           glDeleteProgram {nullptr};
        static PFNGLDETACHSHADERPROC            glDetachShader {nullptr};
        static PFNGLDELETESHADERPROC            glDeleteShader {nullptr};
        static PFNGLDRAWELEMENTSINSTANCEDPROC   glDrawElementsInstanced {nullptr};
        static PFNGLGENERATEMIPMAPPROC          glGenerateMipmap {nullptr};
        static PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback {nullptr};

        void LoadOpenGLFunctions()
        {
			openglDLL = LoadLibraryA("opengl32.dll");
            LoadGLFunction(PFNGLCREATEPROGRAMPROC, glCreateProgram);
            LoadGLFunction(PFNGLDELETETEXTURESPROC, glDeleteTextures);
            LoadGLFunction(PFNGLGENTEXTURESPROC, glGenTextures);
            LoadGLFunction(PFNGLBINDTEXTUREPROC, glBindTexture);
            LoadGLFunction(PFNGLDRAWBUFFERPROC, glDrawBuffer);
            LoadGLFunction(PFNGLDRAWARRAYSPROC, glDrawArrays);
            LoadGLFunction(PFNGLCREATESHADERPROC, glCreateShader);
            LoadGLFunction(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
            LoadGLFunction(PFNGLUNIFORM1FPROC, glUniform1f);
            LoadGLFunction(PFNGLUNIFORM2FVPROC, glUniform2fv);
            LoadGLFunction(PFNGLUNIFORM3FVPROC, glUniform3fv);
            LoadGLFunction(PFNGLUNIFORM1IPROC, glUniform1i);
            LoadGLFunction(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
            LoadGLFunction(PFNGLVERTEXATTRIBDIVISORPROC, glVertexAttribDivisor);
            LoadGLFunction(PFNGLACTIVETEXTUREPROC, glActiveTexture);
            LoadGLFunction(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
            LoadGLFunction(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced);
            LoadGLFunction(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
            LoadGLFunction(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
            LoadGLFunction(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
            LoadGLFunction(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
            LoadGLFunction(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
            LoadGLFunction(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
            LoadGLFunction(PFNGLBLENDFUNCIPROC, glBlendFunci);
            LoadGLFunction(PFNGLBLENDEQUATIONPROC, glBlendEquation);
            LoadGLFunction(PFNGLCLEARBUFFERFVPROC, glClearBufferfv);
            LoadGLFunction(PFNGLSHADERSOURCEPROC, glShaderSource);
            LoadGLFunction(PFNGLCOMPILESHADERPROC, glCompileShader);
            LoadGLFunction(PFNGLGETSHADERIVPROC, glGetShaderiv);
            LoadGLFunction(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
            LoadGLFunction(PFNGLATTACHSHADERPROC, glAttachShader);
            LoadGLFunction(PFNGLLINKPROGRAMPROC, glLinkProgram);
            LoadGLFunction(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
            LoadGLFunction(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
            LoadGLFunction(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
            LoadGLFunction(PFNGLGENBUFFERSPROC, glGenBuffers);
            LoadGLFunction(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
            LoadGLFunction(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
            LoadGLFunction(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
            LoadGLFunction(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
            LoadGLFunction(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
            LoadGLFunction(PFNGLBINDBUFFERPROC, glBindBuffer);
            LoadGLFunction(PFNGLBINDBUFFERBASEPROC, glBindBufferBase);
            LoadGLFunction(PFNGLBUFFERDATAPROC, glBufferData);
            LoadGLFunction(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv);
            LoadGLFunction(PFNGLUSEPROGRAMPROC, glUseProgram);
            LoadGLFunction(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
            LoadGLFunction(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
            LoadGLFunction(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
            LoadGLFunction(PFNGLDETACHSHADERPROC, glDetachShader);
            LoadGLFunction(PFNGLDELETESHADERPROC, glDeleteShader);
            LoadGLFunction(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced);
            LoadGLFunction(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
            LoadGLFunction(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
        }
#pragma endregion

        static PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB {nullptr};
        static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB {nullptr};

        struct ContextInfo
        {
            HGLRC rc {nullptr};
            HDC   dc {nullptr};
            bool  isAlive {false};
        };

        utl::vector<ContextInfo>        contexts;
        utl::vector<id::generationType> generations;
        utl::deque<contextID>           freeIDs;

        i32 activeContextCount {0};

        bool IsAlive(contextID id)
        {
            return contexts[id::GetIndex(id)].isAlive && generations[id::GetIndex(id)] == id::GetGeneration(id);
        }
    } // namespace anonymous
    bool OpenGLInit()
    {

        HWND window {CreateWindowExW(
            0,
            L"Static",
            L"Dummy",
            WS_OVERLAPPEDWINDOW,
            0, 0,
            25, 25,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr)};
        if (!window)
        {
            SM_ASSERT(false, "CreateWindowEx failed");
            return false;
        }
        TRACK_LEAK_ALLOC(window, LeakType::HANDLE, "dummy HWND");

        HDC fakeDC {GetDC(window)};
        if (!fakeDC)
        {
            SM_ASSERT(false, "GetDC failed");
            return false;
        }
        TRACK_LEAK_ALLOC(fakeDC, LeakType::HANDLE, "dummy HDC");

        PIXELFORMATDESCRIPTOR pfd {};
        pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;
        pfd.cDepthBits   = 24;
        pfd.cStencilBits = 8;

        i32 pixelFormat {ChoosePixelFormat(fakeDC, &pfd)};
        if (pixelFormat == 0)
        {
            SM_ASSERT(false, "ChoosePixelFormat failed");
            return false;
        }

        if (!SetPixelFormat(fakeDC, pixelFormat, &pfd))
        {
            SM_ASSERT(false, "SetPixelFormat failed");
            return false;
        }

        HGLRC fakeContext {wglCreateContext(fakeDC)};
        if (!fakeContext)
        {
            SM_ASSERT(false, "wglCreateContext failed");
            return false;
        }
        TRACK_LEAK_ALLOC(fakeContext, LeakType::HANDLE, "dummy HGLRC");

        if (!wglMakeCurrent(fakeDC, fakeContext))
        {
            SM_ASSERT(false, "wglMakeCurrent failed");
            return false;
        }

        LoadOpenGLFunctions(); // Load all OpenGL functions.
        LoadGLFunction(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);
        LoadGLFunction(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
        if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB)
        {
            SM_ASSERT(false, "wglChoosePixelFormatARB or wglCreateContextAttribsARB not found");
            return false;
        }

        // Cleanup dummy stuff.
        wglMakeCurrent(nullptr, nullptr);
        TRACK_LEAK_FREE(fakeContext);
        wglDeleteContext(fakeContext);
        TRACK_LEAK_FREE(fakeDC);
        TRACK_LEAK_FREE(window);
        ReleaseDC(window, fakeDC);
        DestroyWindow(window);

        return true;
    }

    contextID OpenGLCreateContext(const ContextInitInfo* const initInfo)
    {
        contextID id {id::invalidID};

        const i32 pixelAttribs[] {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_ALPHA_BITS_ARB, 8,
            0}; // Last entry must be 0 to terminate the list.

        UINT numPixelFormats {0};
        i32  pixelFormat {0};
        if (!wglChoosePixelFormatARB(initInfo->dc, pixelAttribs, nullptr, 1, &pixelFormat, &numPixelFormats))
        {
            SM_ASSERT(false, "wglChoosePixelFormatARB failed.");
            return id;
        }

        PIXELFORMATDESCRIPTOR pfd {};
        DescribePixelFormat(initInfo->dc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        if (!SetPixelFormat(initInfo->dc, pixelFormat, &pfd))
        {
            SM_ASSERT(false, "SetPixelFormat failed.");
            return id;
        }

        const i32 contextAttribs[] {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            0}; // Last entry must be 0 to terminate the list.

        HGLRC rc {wglCreateContextAttribsARB(initInfo->dc, nullptr, contextAttribs)};
        if (!rc)
        {
            SM_ASSERT(false, "wglCreateContextAttribsARB failed.");
            return id;
        }
        TRACK_LEAK_ALLOC(rc, LeakType::HANDLE, "HGLRC");

        if (!wglMakeCurrent(initInfo->dc, rc))
        {
            SM_ASSERT(false, "wglMakeCurrent failed.");
            return id;
        }

        ContextInfo info {};
        info.rc      = rc;
        info.dc      = initInfo->dc;
        info.isAlive = true;
        if (freeIDs.size() <= id::minDeletedElements)
        {
            id = contextID {(id::idType) contexts.size()};
            contexts.push_back(info);
            generations.push_back(0);
        }
        else
        {
            id = freeIDs.front();
            SM_ASSERT(id::IsValid(id), "freeIDs out of sync.");
            freeIDs.pop_front();
            id                         = contextID {id::NewGeneration(id)};
            contexts[id::GetIndex(id)] = info;
            generations[id::GetIndex(id)]++;
        }

        UpdateWindow(initInfo->handle);
        activeContextCount++;
        return id;
    }

    void BeginFrame(contextID id)
    {
        wglMakeCurrent(contexts[id::GetIndex(id)].dc, contexts[id::GetIndex(id)].rc);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void EndFrame(contextID id)
    {
        SwapBuffers(contexts[id::GetIndex(id)].dc);
    }

    void OpenGLDestroyContext(contextID id)
    {
        if (!IsAlive(id) || !id::IsValid(id))
        {
            SM_ASSERT(false, "Invalid contextID or context is not alive.");
            return;
        }

        ContextInfo& info {contexts[id::GetIndex(id)]};

        wglMakeCurrent(nullptr, nullptr);
        TRACK_LEAK_FREE(info.rc);
        wglDeleteContext(info.rc);
        info.rc      = nullptr;
        info.dc      = nullptr;
        info.isAlive = false;
        freeIDs.push_back(id);
        activeContextCount--;
    }

    void OpenGLShutdown()
    {
		FreeLibrary(openglDLL);

        contexts.clear();
        generations.clear();
        freeIDs.clear();
    }

} // namespace drop::graphics