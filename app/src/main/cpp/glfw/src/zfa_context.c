#include <bits/stdatomic.h>
#include "internal.h"
#include "logger.h"
#include "zomdroid_globals.h"
#include "zomdroid.h"

#define LOG_TAG "zomdroid-glfw"

void makeContextCurrentZfa(_GLFWwindow* window) {
    if (window)
    {
        if (!zfaMakeCurrent(window->context.zfa.handle, _glfw.zomdroid.aNativeWindow,
                            _glfw.zomdroid.aNativeWindow ? _glfw.zomdroid.aNativeWindowWidth : 1,
                            _glfw.zomdroid.aNativeWindow ? _glfw.zomdroid.aNativeWindowHeight : 1)) {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "ZFA: Failed to make context current");
            return;
        }


        /* Vulkan will set native buffer transform based on device orientation, we don't want that,
         * so we rollback to identity transform */
        if (_glfw.zomdroid.aNativeWindow != NULL &&
                ANativeWindow_setBuffersTransform(_glfw.zomdroid.aNativeWindow,
                                          ANATIVEWINDOW_TRANSFORM_IDENTITY) != 0) {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "ZFA: Failed to set native window buffers transform");
            return;
        }
    }

    _glfwPlatformSetTls(&_glfw.contextSlot, window);
}

void swapBuffersZfa(_GLFWwindow* window) {
    pthread_mutex_lock(&g_zomdroid_surface.mutex);
    if (g_zomdroid_surface.is_dirty) {
        g_zomdroid_surface.is_dirty = false;

        _glfw.zomdroid.aNativeWindow = g_zomdroid_surface.native_window;
        _glfw.zomdroid.aNativeWindowWidth = g_zomdroid_surface.width;
        _glfw.zomdroid.aNativeWindowHeight = g_zomdroid_surface.height;

        pthread_mutex_unlock(&g_zomdroid_surface.mutex);

        // finish the frame and signal android ui thread to proceed with surface destruction
        zfaGLFinish();
        pthread_cond_signal(&g_zomdroid_surface.ready_for_destroy_cond);

        makeContextCurrentZfa(window);
        return;
    }
    pthread_mutex_unlock(&g_zomdroid_surface.mutex);

    //if (!_glfw.zomdroid.aNativeWindow) return;

   zfaGLFinish();
    //zfaFlushFront();
    //zfaGLFlush();
}

void swapIntervalZfa(int interval) {

}

GLFWbool extensionSupportedZfa(const char* extension) {
    return GLFW_FALSE;
}

static GLFWglproc getProcAddressZfa(const char* procname)
{
    return (GLFWglproc)_glfwPlatformGetModuleSymbol(_glfw.zfa.handle, procname);
}

static void destroyContextZfa(_GLFWwindow* window)
{
    if (window->context.zfa.handle)
    {
        zfaDestroyContext(window->context.zfa.handle);
        window->context.zfa.handle = NULL;
    }
}

GLFWbool _glfwInitZfa(void)
{
    const char* soname = "libzfa.so";

    if (_glfw.zfa.handle)
        return GLFW_TRUE;

    _glfw.zfa.handle = _glfwPlatformLoadModule(soname);

    if (!_glfw.zfa.handle)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "ZFA: Library not found");
        return GLFW_FALSE;
    }

    _glfw.zfa.CreateContext = (PFN_zfaCreateContext)
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "zfaCreateContext");
    _glfw.zfa.DestroyContext = (PFN_zfaDestroyContext)
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "zfaDestroyContext");
    _glfw.zfa.MakeCurrent = (PFN_zfaMakeCurrent)
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "zfaMakeCurrent");
    _glfw.zfa.GLFinish = (PFN_zfaGLFinish )
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "glFinish");
    _glfw.zfa.GLFlush = (PFN_zfaGLFlush )
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "glFlush");
    _glfw.zfa.FlushFront = (PFN_zfaFlushFront )
            _glfwPlatformGetModuleSymbol(_glfw.zfa.handle, "zfaFlushFront");

    if (!_glfw.zfa.CreateContext ||
        !_glfw.zfa.DestroyContext ||
        !_glfw.zfa.MakeCurrent ||
        !_glfw.zfa.GLFinish ||
        !_glfw.zfa.GLFlush ||
        !_glfw.zfa.FlushFront)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "ZFA: Failed to load required entry points");

        _glfwTerminateZfa();
        return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

void _glfwTerminateZfa(void)
{
    if (_glfw.zfa.handle)
    {
        _glfwPlatformFreeModule(_glfw.zfa.handle);
        _glfw.zfa.handle = NULL;
    }
}

GLFWbool _glfwCreateContextZfa(_GLFWwindow* window,
                                  const _GLFWctxconfig* ctxconfig,
                                  const _GLFWfbconfig* fbconfig)
{
    window->context.zfa.handle =
            zfaCreateContext(fbconfig->depthBits, fbconfig->stencilBits,
                             ctxconfig->profile == GLFW_OPENGL_CORE_PROFILE ? false : true,
                             ctxconfig->major, ctxconfig->minor);

    if (window->context.zfa.handle == NULL)
    {
        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                        "ZFA: Failed to create context");
        return GLFW_FALSE;
    }

    window->context.makeCurrent = makeContextCurrentZfa;
    window->context.swapBuffers = swapBuffersZfa;
    window->context.swapInterval = swapIntervalZfa;
    window->context.extensionSupported = extensionSupportedZfa;
    window->context.getProcAddress = getProcAddressZfa;
    window->context.destroy = destroyContextZfa;

    return GLFW_TRUE;
}