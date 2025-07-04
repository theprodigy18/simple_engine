#include "graphics/opengl/graphics_opengl.hpp"


namespace drop::graphics
{
	bool GraphicsInit()
	{
		return OpenGLInit();
	}

	contextID CreateGraphicsContext(const ContextInitInfo* const initInfo)
	{
		return OpenGLCreateContext(initInfo);
	}

	void DestroyGraphicsContext(contextID id)
	{
        OpenGLDestroyContext(id);
	}

	void GraphicsShutdown()
	{
		OpenGLShutdown();
	}

	void UseContextFrame(contextID id)
	{
		BeginFrame(id);

		EndFrame(id);
	}
} // namespace drop::graphics