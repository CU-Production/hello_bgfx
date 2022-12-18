//
// Created by Admin on 2022/12/18.
//

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include <iostream>

const int WNDW_WIDTH = 1600;
const int WNDW_HEIGHT = 900;

static bool s_showStats = false;

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
}

int main()
{
    std::cout << "Hello world!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "GLFW + bgfx", nullptr, nullptr);
    glfwSetKeyCallback(window, glfw_keyCallback);

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
//    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
//    bgfxInit.type = bgfx::RendererType::Vulkan;
    bgfxInit.type = bgfx::RendererType::Direct3D12;
    bgfxInit.resolution.width = WNDW_WIDTH;
    bgfxInit.resolution.height = WNDW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    // Rendering Loop
    unsigned int counter = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, WNDW_WIDTH, WNDW_HEIGHT);

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        bgfx::frame();

        glfwSwapBuffers(window);
        counter++;
    }

    bgfx::shutdown();
    glfwTerminate();
    return EXIT_SUCCESS;
}
