#include "graphics/opengl/graphics_opengl.hpp"

#include <GL/gl.h>

namespace drop::graphics
{

    namespace
    {
#define LoadGLFunction(type, name)          \
    name = (type) eglGetProcAddress(#name); \
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

        struct ContextInfo
        {
            EGLSurface surface {EGL_NO_SURFACE};
            EGLContext context {EGL_NO_CONTEXT};
            bool       isAlive {false};
        };

        utl::vector<ContextInfo>        contexts;
        utl::vector<id::generationType> generations;
        utl::deque<contextID>           freeIDs;

        i32 activeContextCount {0};

        EGLDisplay display {EGL_NO_DISPLAY};

        bool IsAlive(contextID id)
        {
            return contexts[id::GetIndex(id)].isAlive && generations[id::GetIndex(id)] == id::GetGeneration(id);
        }
    } // namespace anonymous

    bool OpenGLInit()
    {
        EGLDisplay display {eglGetDisplay(EGL_DEFAULT_DISPLAY)};
        if (display == EGL_NO_DISPLAY)
        {
            SM_ASSERT(false, "eglGetDisplay failed");
            return false;
        }
        TRACK_LEAK_ALLOC(display, LeakType::HANDLE, "EGLDisplay");

        if (!eglInitialize(display, nullptr, nullptr))
        {
            SM_ASSERT(false, "eglInitialize failed");
            return false;
        }

        const EGLint configAttribs[] {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE // Terminator.
        };

        EGLConfig config {nullptr};
        EGLint    numConfigs {0};
        if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
        {
            SM_ASSERT(false, "eglChooseConfig failed");
            return false;
        }

        const EGLint pbufferAttribs[] {
            EGL_WIDTH, 1,
            EGL_HEIGHT, 1,
            EGL_NONE // Terminator.
        };

        EGLSurface surface {eglCreatePbufferSurface(display, config, pbufferAttribs)};
        if (surface == EGL_NO_SURFACE)
        {
            SM_ASSERT(false, "eglCreatePbufferSurface failed");
            return false;
        }
        TRACK_LEAK_ALLOC(surface, LeakType::HANDLE, "EGLSurface");

        if (!eglBindAPI(EGL_OPENGL_API))
        {
            SM_ASSERT(false, "eglBindAPI failed");
            return false;
        }

        const EGLint contextAttribs[] {
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 3,
            EGL_NONE // Terminator.
        };

        EGLContext context {eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs)};
        if (context == EGL_NO_CONTEXT)
        {
            SM_ASSERT(false, "eglCreateContext failed");
            return false;
        }
        TRACK_LEAK_ALLOC(context, LeakType::HANDLE, "EGLContext");

        if (!eglMakeCurrent(display, surface, surface, context))
        {
            SM_ASSERT(false, "eglMakeCurrent failed");
            return false;
        }

        LoadOpenGLFunctions(); // Load OpenGL functions.

        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        TRACK_LEAK_FREE(context);
        eglDestroyContext(display, context);
        TRACK_LEAK_FREE(surface);
        eglDestroySurface(display, surface);
        TRACK_LEAK_FREE(display);
        eglTerminate(display);

        return true;
    }

    contextID OpenGLCreateContext(const ContextInitInfo* const initInfo)
    {
        contextID id {id::invalidID};

        // We done this only once because i want to keep abstraction layer not changed just because
        // i need Xlib display to initalize EGLDisplay.
        if (display == EGL_NO_DISPLAY)
        {
            SM_TRACE("Creating EGLDisplay.");
            display = eglGetDisplay((EGLNativeDisplayType) initInfo->dc);
            SM_ASSERT(display != EGL_NO_DISPLAY, "eglGetDisplay failed");
            TRACK_LEAK_ALLOC(display, LeakType::HANDLE, "EGLDisplay");
            EGLint major {4}, minor {3};
            if (!eglInitialize(display, &major, &minor))
            {
                SM_ASSERT(false, "eglInitialize failed");
                return id;
            }
        }

        const EGLint configAttribs[] {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE // Terminator.
        };

        EGLConfig config {nullptr};
        EGLint    numConfigs {0};
        if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
        {
            SM_ASSERT(false, "eglChooseConfig failed");
            return id;
        }

        EGLSurface surface {eglCreateWindowSurface(display, config, (EGLNativeWindowType) initInfo->handle, nullptr)};
        if (surface == EGL_NO_SURFACE)
        {
            SM_ASSERT(false, "eglCreateWindowSurface failed");
            return id;
        }
        TRACK_LEAK_ALLOC(surface, LeakType::HANDLE, "EGLSurface");

        const EGLint contextAttribs[] {
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 3,
            EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
            EGL_NONE // Terminator.
        };

        eglBindAPI(EGL_OPENGL_API);

        EGLContext context {eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs)};
        if (context == EGL_NO_CONTEXT)
        {
            SM_ASSERT(false, "eglCreateContext failed");
            return id;
        }
        TRACK_LEAK_ALLOC(context, LeakType::HANDLE, "EGLContext");

        if (!eglMakeCurrent(display, surface, surface, context))
        {
            SM_ASSERT(false, "eglMakeCurrent failed");
            return id;
        }

        ContextInfo info {};
        info.surface = surface;
        info.context = context;
        info.isAlive = true;

        if (freeIDs.size() <= id::minDeletedElements)
        {
            id = contextID {(id::idType) contexts.size()};
            contexts.push_back(info);
            generations.push_back(0);
        }
        else
        {
            id = freeIDs.back();
            SM_ASSERT(id::IsValid(id), "Invalid contextID.");
            freeIDs.pop_back();
            id                         = contextID {id::NewGeneration(id)};
            contexts[id::GetIndex(id)] = info;
            generations[id::GetIndex(id)]++;
        }

        XFlush(initInfo->dc);
        activeContextCount++;
        return id;
    }

    void OpenGLDestroyContext(contextID id)
    {
        if (!IsAlive(id) || !id::IsValid(id))
        {
            SM_ASSERT(false, "Invalid contextID or context is not alive.");
            return;
        }

        ContextInfo& info {contexts[id::GetIndex(id)]};

        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        TRACK_LEAK_FREE(info.surface);
        eglDestroySurface(display, info.surface);
        TRACK_LEAK_FREE(info.context);
        eglDestroyContext(display, info.context);
        info.surface = EGL_NO_SURFACE;
        info.context = EGL_NO_CONTEXT;
        info.isAlive = false;
        freeIDs.push_back(id);
        activeContextCount--;
    }

    void BeginFrame(contextID id)
    {
        ContextInfo& info {contexts[id::GetIndex(id)]};
        eglMakeCurrent(display, info.surface, info.surface, info.context);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void EndFrame(contextID id)
    {
        eglSwapBuffers(display, contexts[id::GetIndex(id)].surface);
    }

    void OpenGLShutdown()
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        TRACK_LEAK_FREE(display);
        eglTerminate(display);
        display = EGL_NO_DISPLAY;

        contexts.clear();
        generations.clear();
        freeIDs.clear();
    }

} // namespace drop::graphics